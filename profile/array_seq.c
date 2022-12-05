#include "array_seq.c"
#include <stdlib.h>
#include <stdio.h>


// /* uint arr */
// struct seg_arr {
//         uint32_t *segment;
//         int length;
// }

// /* uarray new */

// seg_arr seg_arr_new(int length) {
//         /* Mallocs a uarray */
//         seg_arr seg = malloc(sizeof(*seg));
//         /* */
//         seg->segment = malloc(sizeof(uint32_t) * length);
//         seg->length = length;
//         return seg;
// }

// /* uarray at */
// void seg_arr_add(seg_arr seg, uint32_t word, int index) {
//         seg->segment[index] = word;
// }


// /* uarray length */
// int seg_arr_length(seg_arr seg) {
//         return seg->length;
// }

// /* free */
// void seg_arr_free(seg_arr seg) {
//         free(seg->segment); 
//         free(seg);
// }



/* Struct definition of a Memory_T which 
   contains two sequences: 
   - one holding pointers to UArray_T's representing segments
   - one holding pointers to uint32_t's representing free segments */
struct Memory_T {
        //Seq_T segments;
      // uint32_t *segments;
       //Seq_T free;
        seg_arr *segments; 
        uint32_t mem_length;
        uint32_t seg_capacity; 
        uint32_t *free;
        uint32_t unmapped_length; 
        uint32_t unampped_capacity; 
        uint32_t total_segments; 
};



/* new */
seg_arr *mem_arr_new(int length) {
        // fprintf(stderr, "making new segment\n");
    seg_arr *segments = malloc(length * sizeof(struct seg_arr)); 
    assert (segments != NULL);
    return segments;
}



/* seq get */
seg_arr mem_arr_get(Memory_T m, uint32_t index) {
        // fprintf(stderr, "get index: %u\n", index);
        assert(index < m->seg_capacity);
        return m->segments[index]; 
}



/* seq put */
void mem_arr_put(Memory_T m, seg_arr seg, int index) 
{
        // if (index >= m->seg_capacity) {
        //         m->segments = realloc(m->segments, m->seg_capacity * 2 * sizeof(uint32_t) + sizeof(uint32_t));

        // }
        // fprintf(stderr, "put\n");
        m->segments[index] = seg;
}



/* add hi */
void mem_arr_addhi(Memory_T m, seg_arr seg)
{
        assert(seg != NULL); 
        if (m->total_segments >= m->seg_capacity) {
                m->segments = realloc(m->segments, m->seg_capacity * 2 * sizeof(struct seg_arr) + sizeof(uint32_t));
                m->seg_capacity = (m->seg_capacity * 2) + 1; 
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