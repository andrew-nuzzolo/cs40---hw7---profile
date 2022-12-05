#include "array_seq.c"
#include <stdlib.h>
#include <stdio.h>

/* Struct definition of a Memory_T which 
   contains two sequences: 
   - one holding pointers to UArray_T's representing segments
   - one holding pointers to uint32_t's representing free segments */
struct Memory_T {
        seg_arr *segments; 
        uint32_t mem_length;
        uint32_t seg_capacity; 
        uint32_t *free;
        uint32_t unmapped_length; 
        uint32_t unampped_capacity; 
        uint32_t total_segments; 
};



/* seq get */
seg_arr mem_arr_get(Memory_T m, uint32_t index) {
        // fprintf(stderr, "get index: %u\n", index);
        assert(index < m->seg_capacity);
        return m->segments[index]; 
}



/* seq put */
void mem_arr_put(Memory_T m, seg_arr seg, int index) 
{
        m->segments[index] = seg;
}



/* add hi */
void mem_arr_addhi(Memory_T m, seg_arr seg)
{
        assert(seg != NULL); 
        if (m->total_segments >= m->seg_capacity) {
                m->segments = realloc(m->segments, m->seg_capacity * 2 * sizeof(struct seg_arr) + sizeof(uint32_t));
                
                uint32_t old_cap = m->seg_capacity;
                m->seg_capacity = (m->seg_capacity * 2) + 1;
                uint32_t new_cap = m->seg_capacity;

                for (uint32_t i = old_cap; i < new_cap; ++i) {
                        m->segments[i] = 0;
                }
        }
        m->segments[m->total_segments - 1] = seg;
        m->mem_length++; 
        // fprintf(stderr, "addhi seg\n");
}



/* free */
void mem_arr_free(Memory_T m) 
{
        // fprintf(stderr, "freeing memory\n");
        assert(m != NULL);
        for (uint32_t i = 0; i < m->seg_capacity; i++) {
                seg_arr temp = m->segments[i];
                if (temp != NULL) {
                        if (temp->segment != NULL) {
                                free(temp->segment); 
                                m->mem_length--; 
                        }
                        free(temp); 
                }
                
                if (m->mem_length == 0) {
                        break;
                }
        }
}

/* length */
int mem_arr_length(Memory_T m) {
        // fprintf(stderr, "memory_array_length: %d\n", m->mem_length);
        return m->mem_length;
        
}