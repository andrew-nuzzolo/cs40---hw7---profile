/*
 * Implementation for the UM segmented memory, which includes functions for
 * allocation/deallocation of memory, mapping/unmapping segments, and
 * interacting with segments (get/put)
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h" 
#include "uarray.h"
#include "seq.h"
#define HINT 10
#define BIG_HINT 1000


typedef struct seg_arr *seg_arr; 
struct seg_arr {
        uint32_t *segment;
        int length;
};

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


/* Name: memory_new
 * Input: a uint32_t representing the length of segment zero
 * Output: A newly allocated Memory_T struct
 * Does: * Creates a Sequence of UArray_T's, sets all segments to NULL besides
 *         segment 0, and segment 0 is initialized to be "length" long
         * Creates a Sequence of uint32_t pointers and 
           sets them to be the index of the unmapped segments
 * Error: Asserts if memory is not allocated
 */
Memory_T memory_new(uint32_t length)
{
        Memory_T m_new = malloc(sizeof(*m_new));
        assert(m_new != NULL);

        /* Creating the segments */
        // m_new->segments = Seq_new(HINT);
        // assert(m_new->segments != NULL);
        m_new->segments = mem_arr_new(BIG_HINT);

        /* Creating the sequence to keep track of free segments */
        // m_new->free = Seq_new(HINT);
        // assert(m_new->free != NULL);

        m_new->free = malloc(sizeof(uint32_t) * BIG_HINT); 
        assert(m_new->free != NULL); 

        m_new->unmapped_length = 0; 
        m_new->unampped_capacity = BIG_HINT; 
        m_new->total_segments = 0; 
        
        /* Creating segment zero with proper length*/
        memory_map(m_new, length);

        return m_new;
}

/* Name: memory_free
 * Input: A pointer to a Memory_T struct 
 * Output: N/A 
 * Does: Frees all memory associated with the struct
 * Error: Asserts if struct is NULL
 */
void memory_free(Memory_T *m)
{
        assert(*m != NULL);

        /* Freeing the UArray_T segments */
       // int seg_len = Seq_length((*m)->segments);
        // for (int seg_num = 0; seg_num < seg_len; ++seg_num) {
        //         // UArray_T aux = (UArray_T)Seq_remlo((*m)->segments);
        //         seg_arr aux = (seg_arr)Seq_remlo((*m)->segments);

        //         /* If the segment is unmapped, there is nothing to free */
        //         if (aux == NULL) {
        //                 continue;
        //         } else {
        //                 // UArray_free(&aux);
        //                 // seg_arr_free(aux); // not sure if right
        //                 free(aux->segment); 
        //                 free(aux);
        //         }
        // }

        mem_arr_free(*m);

        /* Freeing everything else */
        //Seq_free(&(*m)->segments);
        // Seq_free(&(*m)->free); 
        free((*m)->free);
        free(*m);
}

/* Name: memory_put
 * Input: A Memory_T struct, a segment number, an offset, and a value
 * Output: N/A
 * Does: Inserts value at the specificed segment and offset
 * Error: Asserts if struct is NULL
 *        Asserts if segment is not mapped
 *        Asserts if offset is not mapped
 */
void memory_put(Memory_T m, uint32_t seg, uint32_t off, uint32_t val)
{
       // fprintf(stderr, "adding word\n"); 
        assert(m != NULL);
        // assert(seg < (uint32_t)Seq_length(m->segments));

        // UArray_T queried_segment = (UArray_T)Seq_get(m->segments, seg);
        // seg_arr queried_segment = (seg_arr)Seq_get(m->segments, seg);
        // assert(queried_segment != NULL);
        seg_arr queried_segment = mem_arr_get(m, seg);
        assert(queried_segment != NULL);

        // assert(off < (uint32_t)UArray_length(queried_segment));
        assert(off < (uint32_t)queried_segment->length);


        // *(uint32_t *)UArray_at(queried_segment, off) = val;
        // seg_arr_add(queried_segment, val, off);
        queried_segment->segment[off] = val;

}

/* Name: memory_get
 * Input: A Memory_T struct, a segment number, and an offset
 * Output: A uint32_t which represents the value at that segment and offset
 * Does: Gets the value at the specified segment number and offset and returns
 * Error: Asserts if struct is NULL
 *        Asserts if segment is not mapped
 *        Asserts if offset is not mapped
 */
uint32_t memory_get(Memory_T m, uint32_t seg, uint32_t off)
{
        assert(m != NULL);
        // assert(seg < (uint32_t)Seq_length(m->segments));
        
        // UArray_T queried_segment = (UArray_T)Seq_get(m->segments, seg);
        // seg_arr queried_segment = (seg_arr)Seq_get(m->segments, seg);
        seg_arr queried_segment = mem_arr_get(m, seg);

        assert(queried_segment != NULL);

        // assert(off < (uint32_t)UArray_length(queried_segment));
        assert(off < (uint32_t)queried_segment->length);

        // return *(uint32_t *)UArray_at(queried_segment, off);
        return queried_segment->segment[off];

}

/* Name: memory_map
 * Input: A Memory_T struct, a segment number, and segment length
 * Output: the index of the mapped segment
 * Does: Creates a segment that is "length" long 
 *       with all of the segment's values being zero and 
 *       returns index of the mapped segment
 * Error: Asserts if struct is NULL
 *        Asserts if memory for segment is not allocated
 */
uint32_t memory_map(Memory_T m, uint32_t length)
{
        //fprintf(stderr, "memory_map\n"); 
        assert(m != NULL);

        // UArray_T seg = UArray_new(length, sizeof(uint32_t));
        // seg_arr seg = seg_arr_new(length);
        seg_arr seg = malloc(sizeof(struct seg_arr));
        assert(seg != NULL);
        seg->segment = malloc(sizeof(uint32_t) * length);
        assert(seg->segment != NULL);
        seg->length = length;


        /* Setting values in new segment to 0 */
        for (uint32_t arr_index = 0; arr_index < length; ++arr_index) {
                // *(uint32_t *)UArray_at(seg, arr_index) = 0;
                // seg_arr_add(seg, 0, arr_index);
                seg->segment[arr_index] = 0;
        }

        if (m->unmapped_length == 0) {
                m->total_segments++; 
                // Seq_addhi(m->segments, seg); 
                mem_arr_addhi(m, seg);
                return m->total_segments - 1;  
        } else {
                m->unmapped_length--;
               // Seq_put(m->segments, m->free[m->unmapped_length], seg); 
                mem_arr_put(m, seg, m->free[m->unmapped_length]);
                // assert(Seq_get(m->segments, m->free[m->unmapped_length]) != NULL);
                assert(mem_arr_get(m, m->free[m->unmapped_length]) != NULL);

                return m->free[m->unmapped_length]; 
        }

}

/* Name: memory_unmap
 * Input: A Memory_T struct and a segment number
 * Output: N/A
 * Does: Unmaps a specified segment at the "seg_num" index and frees memory
 *       of the associated segment as well as adds that index back into the
 *       free segment sequence
 * Error: Asserts if struct is NULL
 *        Asserts if unmap segment 0s
 *        Asserts if segment is NULL
 */
void memory_unmap(Memory_T m, uint32_t seg_num)
{
        //fprintf(stderr, "memory_unmap");
        assert(m != NULL);
        assert(seg_num != 0);

        // UArray_T unmap = Seq_get(m->segments, seg_num);
        // seg_arr unmap = Seq_get(m->segments, seg_num);
        seg_arr unmap = mem_arr_get(m, seg_num);

        assert(unmap != NULL);

        // UArray_free(&unmap);
        // seg_arr_free(unmap);
        free(unmap->segment); 
        free(unmap);

        if (m->unmapped_length == m->unampped_capacity) {
                m->free = realloc(m->free, m->unampped_capacity * 2 * sizeof(uint32_t) + sizeof(uint32_t));
                m->unampped_capacity = m->unampped_capacity * 2 + 1;
        }

        m->free[m->unmapped_length] = seg_num; 
        m->unmapped_length++; 

       // Seq_put(m->segments, seg_num, NULL);
        mem_arr_put(m, NULL, seg_num);

        // m->mem_length--;
}

void load_program(Memory_T m, uint32_t seg_num)
{
        assert(m != NULL); 

        if (seg_num == 0) {
                return; 
        }

        // seg_arr to_copy = (seg_arr)Seq_get(m->segments, seg_num);
        seg_arr to_copy = mem_arr_get(m, seg_num);
        assert(to_copy != NULL); 

        seg_arr copy = malloc(sizeof(*copy)); 
        assert(copy != NULL); 
        copy->segment = malloc(sizeof(uint32_t) * to_copy->length);
        assert(copy->segment != NULL);
        copy->length = to_copy->length;
      

        for (int i = 0; i < to_copy->length; i++) {
                copy->segment[i] = to_copy->segment[i]; 
        }

        // seg_arr seg_0 = (seg_arr)Seq_get(m->segments, 0);
        seg_arr seg_0 = mem_arr_get(m, 0);

        
        free(seg_0->segment); 
        free(seg_0);

       // Seq_put(m->segments, 0, copy);
        mem_arr_put(m, copy, 0);
     
}

/*
 * Implementation of the full UM, which includes functions to
 * allocate/deallocate memory associated with the UM_T struct,
 * function to populate segment zero with an instruction,
 * functions to execute instructions in segment zero,
 * and functions to perform all UM instructions
 *
 */

// #include <assert.h>
// #include <stdio.h>
// #include <stdlib.h>

// #include "um.h"
// #include "uarray.h" 
// #include "seq.h"
#include "bitpack.h"
// #include "registers.h"
// #include "memory.h"

#define WORD_SIZE 32
#define OP_WIDTH 4
#define R_WIDTH 3
#define RA_LSB 6
#define RB_LSB 3
#define RC_LSB 0
#define NUM_REGISTERS 8
#define VALUE_SIZE 25

static uint32_t registers[NUM_REGISTERS];

// /* Struct definition of a Memory_T which 
//    contains two sequences: 
//    - one holding pointers to UArray_T's representing segments
//    - one holding pointers to uint32_t's representing free segments */
// struct Memory_T {
//         Seq_T segments;
//         Seq_T free;
// };


/* Name: input
 * Input:register c
 * Output: N/A
 * Does: takes in input from stdin and loads val into rc
 *       If end of input is signalled,
 *            rc gets 32-bit word in which every bit is 1 (~0)
 * Note: since we used fgetc, the inputted value can never be greater than 255
 */
void input(uint32_t rc) 
{
    int input = fgetc(stdin);
    if (input == EOF) { 
            registers[rc] = ~0;
    } else {
            registers[rc] = input;       
    }
}

// /* Name: load_program
//  * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
//  * Output: a uint32_t representing the index that program should start at
//  * Does: copies segment[rb value] into segment zero and returns rc value
//  * Error: Asserts UM_T struct is NULL
//  *        Asserts if any register number is valid
//  */
// uint32_t load_program(Memory_T mem, uint32_t rb, uint32_t rc)
// {
//     assert(mem != NULL);

//     /* If rb value is 0, 0 is already loaded into segment 0 */
//     if (registers[rb] == 0) {
//         return registers[rc];
//     }
    
//     /* Get the segment to copy */
//     UArray_T to_copy = (UArray_T)Seq_get(mem->segments, registers[rb]);
//     assert(to_copy != NULL);

//     /* Creating a copy with the same specifications */
//     int seg_len = UArray_length(to_copy);
//     UArray_T copy = UArray_new(seg_len, UArray_size(to_copy));
//     assert(copy != NULL);

//     /* Deep copying */
//     for (int i = 0; i < seg_len; i++){
//         *(uint32_t *)UArray_at(copy, i) = 
//         *(uint32_t *)UArray_at(to_copy, i);
//     }

//     /* Freeing segment 0 and inserting the copy */
//     UArray_T seg_zero = (UArray_T)Seq_get(mem->segments, 0);
//     UArray_free(&seg_zero);
//     Seq_put(mem->segments, 0, copy);

//     return registers[rc];
// }

/* Name: instruction_call
 * Input: UM_T struct of the um
 *        opcode of instruction to be executed
 *        unint32_t's representing registers A, B, C
 * Output: N/A
 * Does: Executes opcodes 0 to 11 (cmove to input)
 * Error: Asserts if opcode is invalid
 *        Asserts if any register number is valid
 *        Asserts if UM_T sruct is NULL
 * Notes: is called by um_execute
 */
void instruction_call(Memory_T mem, Um_opcode op, uint32_t ra, 
                      uint32_t rb, uint32_t rc)
{
    assert(op >= 0 && op < 14); //DO WE NEED IT
    assert(mem != NULL);

    switch (op) {
        case CMOV: 
                if (registers[rc] != 0) {
                        registers[ra] = registers[rb];
                }
                break;
        case SLOAD: 
                // fprintf(stderr, "sload\n");
                registers[ra] = memory_get(mem, registers[rb], registers[rc]);  
                break;
        case SSTORE: 
                        // fprintf(stderr, "sstore\n");

                memory_put(mem, registers[ra], registers[rb], registers[rc]);;  
                break;
        case ADD: 
                registers[ra] = registers[rb] + registers[rc]; 
                break;
        case MUL: 
                registers[ra] = registers[rb] * registers[rc];                 
                break;
        case DIV: 
                registers[ra] = registers[rb] / registers[rc];                  
                break;
        case NAND: 
                registers[ra] = ~(registers[rb] & registers[rc]);                   
                break;
        case HALT: 
                memory_free(&mem);
                exit(EXIT_SUCCESS);
                break;
        case MAP: 
                        // fprintf(stderr, "map func\n");

                registers[rb] = memory_map(mem, registers[rc]); 
                break;
        case UNMAP: 
                        // fprintf(stderr, "unmap func\n");

                memory_unmap(mem, registers[rc]);
                break;
        case OUT: 
                putchar(registers[rc]);
                break;
        case IN: 
                input(rc);
                break;

        default: assert(true);
    }
}


/* Name: um_execute
 * Input: a UM_T struct
 * Output: N/A
 * Does: Executes all instructions in segment zero until
 *       there is no instruction left or until there is a halt instruction
 *       Calls instruction_call to execute all instructions except
 *       load program and load value
 * Error: Asserts if UM_T struct is NULL
 *        Asserts if segment zero is NULL at any point
 */
void um_execute(Memory_T mem)
{
        assert(mem != NULL);
        // fprintf(stderr, "executing instrs\n");

        // UArray_T seg_zero = (UArray_T)Seq_get(mem->segments, 0);
        // seg_arr seg_zero = (seg_arr)Seq_get(mem->segments, 0);
        seg_arr seg_zero = mem_arr_get(mem, 0);


        assert(seg_zero != NULL);

        // int seg_zero_len = UArray_length(seg_zero);
         int seg_zero_len = seg_zero->length;
        //int seg_zero_len = mem_arr_length(mem);
        int prog_counter = 0;
        uint32_t opcode, ra, rb, rc, word;

        /* Execute words in segment zero until there are none left */
        while (prog_counter < seg_zero_len) {
                // word = *(uint32_t *)UArray_at(seg_zero, prog_counter);
                word = seg_zero->segment[prog_counter];
                opcode = word >> (WORD_SIZE - OP_WIDTH);
                prog_counter++;

                /* Load value */
                if (opcode == 13) {
                                       // fprintf(stderr, "loadval\n");

                        ra = (word << (WORD_SIZE - (R_WIDTH + VALUE_SIZE))) >> (WORD_SIZE - R_WIDTH); 
                        registers[ra] = (word << (WORD_SIZE - VALUE_SIZE)) >> (WORD_SIZE - VALUE_SIZE); 
                        continue;
                } 

                ra = (word << (WORD_SIZE - (R_WIDTH + RA_LSB))) >> (WORD_SIZE - R_WIDTH); 
                rb = (word << (WORD_SIZE - (R_WIDTH + RB_LSB))) >> (WORD_SIZE - R_WIDTH); 
                rc = (word << (WORD_SIZE - (R_WIDTH + RC_LSB))) >> (WORD_SIZE - R_WIDTH); 

                /* Load Program */
                if (opcode == 12) {
                                        // fprintf(stderr, "load_program\n");

                        /* Updates programs counter*/
                        prog_counter = registers[rc];
                        // fprintf(stderr, "called load program\n");

                        load_program(mem, registers[rb]);


                        // seg_zero = (seg_arr)Seq_get(mem->segments, 0);
                        seg_zero = mem_arr_get(mem, 0);
                        
                        assert(seg_zero != NULL);

                        seg_zero_len = seg_zero->length;
                } else {
                        instruction_call(mem, opcode, ra, rb, rc);
                }
        }
}
