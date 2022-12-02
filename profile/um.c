/*
 * Implementation of the full UM, which includes functions to
 * allocate/deallocate memory associated with the UM_T struct,
 * function to populate segment zero with an instruction,
 * functions to execute instructions in segment zero,
 * and functions to perform all UM instructions
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "um.h"
#include "uarray.h" 
#include "seq.h"
#include "bitpack.h"
// #include "registers.h"
#include "memory.h"

#define WORD_SIZE 32
#define OP_WIDTH 4
#define R_WIDTH 3
#define RA_LSB 6
#define RB_LSB 3
#define RC_LSB 0
#define NUM_REGISTERS 8
// #define VALUE_SIZE (WORD_SIZE - OP_WIDTH - R_WIDTH)

static uint32_t registers[NUM_REGISTERS];

/* NEED TO REMOVE THIS AND CALLS FROM UM_DRIVER */
// struct UM_T {
//     Registers_T reg;
//     Memory_T mem;
// };

/* Struct definition of a Memory_T which 
   contains two sequences: 
   - one holding pointers to UArray_T's representing segments
   - one holding pointers to uint32_t's representing free segments */
struct Memory_T {
        Seq_T segments;
        Seq_T free;
};

/* Name: um_new
 * Input: a uint32_t representing the length of segment zero
 * Output: A newly allocated UM_T struct
 * Does: Allocates memory for a UM_T
 *       Creates a new Registers_T member and Memory_T member
 * Error: Asserts if memory is not allocated
 */
// Memory_T um_new(uint32_t length)
// {
//     Memort_T new = malloc(sizeof(*um_new));
//     assert(um_new != NULL);
//     um_new->mem = memory_new(length);

//     return um_new;
// }

/* Name: um_free
 * Input: A pointer to a UM_T struct 
 * Output: N/A 
 * Does: Frees all memory associated with the struct and its members
 * Error: Asserts if UM_T struct is NULL
 */
// void um_free(UM_T *um)
// {
//     assert((*um) != NULL);

//     registers_free(&(*um)->reg);
//     memory_free(&(*um)->mem);
//     free(*um);
// }

 /* Name: conditional_move
 * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
 * Output: N/A
 * Does: if value in rc is not 0, moves value of rb into ra
 * Error: Asserts if UM_T struct is NULL
 *        Asserts if any register number is valid
 */
// void conditional_move(uint32_t ra, uint32_t rb, uint32_t rc)
// {
//     if (registers[rc] != 0) {
//         registers[ra] = registers[rb];
//     }
// }

/* Name: segmented_load
 * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
 * Output: N/A
 * Does: register a gets value of segment(val in rb) at offset(val in rc)
 * Error: Asserts if UM_T struct is NULL
 *        Asserts if any register number is valid
 */
// void segmented_load(Memory_T mem, uint32_t ra, uint32_t rb, uint32_t rc)
// {
//     assert(mem != NULL);
//     registers[ra] = memory_get(mem, registers[rb], registers[rc]);
// }

 /* Name: segmented_store
 * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
 * Output: N/A
 * Does: stores val in rc in segment(val in ra) at offset(val in rb)
 * Error: Asserts if UM_T struct is NULL
 *        Asserts if any register number is valid
//  */
// void segmented_store(Memory_T mem, uint32_t ra, uint32_t rb, uint32_t rc)
// {
//     assert(mem != NULL);
//     // assert(ra < NUM_REGISTERS && rb < NUM_REGISTERS && rc < NUM_REGISTERS);
//     // memory_put(um->mem, ra_val, rb_val, registers_get(um->reg, rc));
//     memory_put(mem, registers[ra], registers[rb], registers[rc]);
// }

/* Name: add
 * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
 * Output: N/A
 * Does: adds values in rb and rc, and stores (sum % 2 ^ 32) in ra
 * Error: Asserts if UM_T struct is NULL
 *        Asserts if any register number is valid
 */
// void add(uint32_t ra, uint32_t rb, uint32_t rc)
// {
//      registers[ra] = registers[rb] + registers[rc];      
// }

// /* Name: multiply
//  * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
//  * Output: N/A
//  * Does: multiplies values in rb and rc, and stores (product % 2 ^ 32) in ra
//  * Error: Asserts if UM_T struct is NULL
//  *        Asserts if any register number is valid
//  */
// void multiply(uint32_t ra, uint32_t rb, uint32_t rc)
// {
//     registers[ra] = registers[rb] * registers[rc];      
// }

// /* Name: divide
//  * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
//  * Output: N/A
//  * Does: computes (value in rb) / (value in rc), and stores quotient in ra
//  * Error: Asserts if UM_T struct is NULL
//  *        Asserts if any register number is valid
//  */
// void divide(uint32_t ra, uint32_t rb, uint32_t rc)
// {
//     // assert(um != NULL);
//     // assert(ra < NUM_REGISTERS && rb < NUM_REGISTERS && rc < NUM_REGISTERS);

//     // uint32_t rb_val = registers_get(um->reg, rb);
//     // uint32_t rc_val = registers_get(um->reg, rc);
//     // assert(rc_val != 0);

//     // registers_put(um->reg, ra, (rb_val / rc_val));

//     registers[ra] = registers[rb] / registers[rc];      

// }

// /* Name: nand
//  * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
//  * Output: N/A
//  * Does: performs bitwise and on val in rb and val in rc,
//  *       and stores the not (~) of the result in ra
//  * Error: Asserts if UM_T struct is NULL
//  *        Asserts if any register number is valid
//  */
// void nand(uint32_t ra, uint32_t rb, uint32_t rc)
// {
//     registers[ra] = ~(registers[rb] & registers[rc]);
// }

/* Name: halt
 * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
 * Output: N/A
 * Does: frees all memory associated with the um and exits program
 * Error: Asserts if UM_T struct is NULL
 *        Asserts if any register number is valid
 */
// void halt(Memory_T mem)
// {
//     memory_free(&mem);
//     exit(EXIT_SUCCESS);
// }

/* Name: map_segment
 * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
 * Output: N/A
 * Does: maps a segment of length of val in rc
 *       and stores index number of segment in rb
 * Error: Asserts if UM_T struct is NULL
 *        Asserts if any register number is valid
 */
// void map_segment(Memory_T mem, uint32_t rb, uint32_t rc)
// {
//     assert(mem != NULL);

//     // uint32_t rc_val = registers_get(um->reg, rc);

//     // uint32_t index = memory_map(um->mem, rc_val);
//     // registers_put(um->reg, rb, index);

//     registers[rb] = memory_map(mem, registers[rc]); 
// }

/* Name: unmap_segment
 * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
 * Output: N/A
 * Does: unmaps segment at value in rc
 * Error: Asserts if UM_T struct is NULL
 *        Asserts if any register number is valid
 */
// void unmap_segment(Memory_T mem, uint32_t rc)
// {
//     assert(mem != NULL);
  

//     memory_unmap(mem, registers[rc]);
// }

// /* Name: output
//  * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
//  * Output: N/A
//  * Does: outputs the value in rc
//  * Error: Asserts if UM_T struct is NULL
//  *        Asserts if any register number is valid
//  *        Asserts if value in rc is not valid (not between 0 to 255 inclusive)
//  */
// void output(uint32_t rc)
// {
//     // assert(um != NULL);
//     // assert(ra < NUM_REGISTERS && rb < NUM_REGISTERS && rc < NUM_REGISTERS);

//     // uint32_t rc_val = registers_get(um->reg, rc);
//     // assert(rc_val < 256);

//     // putchar(rc_val);

//     putchar(registers[rc]);
// }

/* Name: input
 * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
 * Output: N/A
 * Does: takes in input from stdin and loads val into rc
 *       If end of input is signalled,
 *            rc gets 32-bit word in which every bit is 1 (~0)
 * Error: Asserts if UM_T struct is NULL
 *        Asserts if any register number is valid
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

/* Name: load_program
 * Input: UM_T struct; uint32_t's reprsenting registers A, B, C
 * Output: a uint32_t representing the index that program should start at
 * Does: copies segment[rb value] into segment zero and returns rc value
 * Error: Asserts UM_T struct is NULL
 *        Asserts if any register number is valid
 */
uint32_t load_program(Memory_T mem, uint32_t rb, uint32_t rc)
{
    assert(mem != NULL);

    /* If rb value is 0, 0 is already loaded into segment 0 */
    if (registers[rb] == 0) {
        return registers[rc];
    }
    
    /* Get the segment to copy */
    UArray_T to_copy = (UArray_T)Seq_get(mem->segments, registers[rb]);
    assert(to_copy != NULL);

    /* Creating a copy with the same specifications */
    int seg_len = UArray_length(to_copy);
    UArray_T copy = UArray_new(seg_len, UArray_size(to_copy));
    assert(copy != NULL);

    /* Deep copying */
    for (int i = 0; i < seg_len; i++){
        *(uint32_t *)UArray_at(copy, i) = 
        *(uint32_t *)UArray_at(to_copy, i);
    }

    /* Freeing segment 0 and inserting the copy */
    UArray_T seg_zero = (UArray_T)Seq_get(mem->segments, 0);
    UArray_free(&seg_zero);
    Seq_put(mem->segments, 0, copy);

    return registers[rc];
}

/* Name: load_value
 * Input: a UM_T struct, a register number, and a value
 * Output: N/A
 * Does: loads the passed in value into register ra
 * Error: Asserts if UM_T struct is NULL
 *        Asserts if register is invalid
 */
// void load_value(uint32_t ra, uint32_t val)
// {
//                        registers[ra] = value; 

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
    assert(op >= 0 && op < 14);
    assert(mem != NULL);

    switch (op) {
        case CMOV: 
                if (registers[rc] != 0) {
                        registers[ra] = registers[rb];
                }
                break;
        case SLOAD: 
                registers[ra] = memory_get(mem, registers[rb], registers[rc]);  
                break;
        case SSTORE: 
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
                registers[rb] = memory_map(mem, registers[rc]); 
                break;
        case UNMAP: 
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

    UArray_T seg_zero = (UArray_T)Seq_get(mem->segments, 0);
    assert(seg_zero != NULL);

    int seg_zero_len = UArray_length(seg_zero);
    int prog_counter = 0;
    uint32_t opcode, ra, rb, rc, word;

    /* Execute words in segment zero until there are none left */
    while (prog_counter < seg_zero_len) {
        word = *(uint32_t *)UArray_at(seg_zero, prog_counter);
        
        //opcode = Bitpack_getu(word, OP_WIDTH, WORD_SIZE - OP_WIDTH);
        opcode = word >> (WORD_SIZE - OP_WIDTH);

        prog_counter++;

        /* Load value */
        if (opcode == 13) {
                uint32_t value_size = WORD_SIZE - OP_WIDTH - R_WIDTH;
                ra = Bitpack_getu(word, R_WIDTH, value_size);
        
                uint32_t value = Bitpack_getu(word, value_size, 0);
                registers[ra] = value; 
                continue;
        } 

        ra = Bitpack_getu(word, R_WIDTH, RA_LSB);

        rb = Bitpack_getu(word, R_WIDTH, RB_LSB);
        
        rc = Bitpack_getu(word, R_WIDTH, RC_LSB);

        /* Load Program */
        if (opcode == 12) {
            /* Updates programs counter*/
            prog_counter = load_program(mem, rb, rc);

            seg_zero = (UArray_T)Seq_get(mem->segments, 0);
            assert(seg_zero != NULL);

            seg_zero_len = UArray_length(seg_zero);
        } else {
            instruction_call(mem, opcode, ra, rb, rc);
        }
    }
}


/* Name: populate
 * Input: UM_T struct,
 *        uint32_t "index" representing offset of segment,
 *        uint32_t "word" representing word to put into
 *            segment zero at given offset
 * Output: N/A
 * Does: Populates segment zero at offset "index" with value "word"
 * Error: Asserts if the UM_T struct is NULL
 * Notes: called by driver to populate segment zero with all instructions
 */
void populate(Memory_T mem, uint32_t index, uint32_t word)
{
    assert(mem != NULL);
    memory_put(mem, 0, index, word);
}
