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

// #include "um.h"
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
#define VALUE_SIZE 25

static uint32_t registers[NUM_REGISTERS];

/* Struct definition of a Memory_T which 
   contains two sequences: 
   - one holding pointers to UArray_T's representing segments
   - one holding pointers to uint32_t's representing free segments */
struct Memory_T {
        Seq_T segments;
        Seq_T free;
};


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

        // UArray_T seg_zero = (UArray_T)Seq_get(mem->segments, 0);
        seg_arr seg_zero = (seg_arr)Seq_get(mem->segments, 0);;

        assert(seg_zero != NULL);

        // int seg_zero_len = UArray_length(seg_zero);
        int seg_zero_len = seg_arr_length(seg_zero);

        int prog_counter = 0;
        uint32_t opcode, ra, rb, rc, word;

        /* Execute words in segment zero until there are none left */
        while (prog_counter < seg_zero_len) {
                // word = *(uint32_t *)UArray_at(seg_zero, prog_counter);
                word = seg_zero[prog_counter];

                //opcode = Bitpack_getu(word, OP_WIDTH, WORD_SIZE - OP_WIDTH);
                opcode = word >> (WORD_SIZE - OP_WIDTH);

                prog_counter++;

                /* Load value */
                if (opcode == 13) {
                        //uint32_t value_size = WORD_SIZE - OP_WIDTH - R_WIDTH;
                // ra = Bitpack_getu(word, R_WIDTH, VALUE_SIZE);
                        ra = (word << (WORD_SIZE - (R_WIDTH + VALUE_SIZE))) >> (WORD_SIZE - R_WIDTH); 
                        // uint32_t value = Bitpack_getu(word, VALUE_SIZE, 0);
                        //uint32_t value = (word << (WORD_SIZE - VALUE_SIZE)) >> (WORD_SIZE - VALUE_SIZE); 
                        registers[ra] = (word << (WORD_SIZE - VALUE_SIZE)) >> (WORD_SIZE - VALUE_SIZE); 
                        continue;
                } 

                //ra = Bitpack_getu(word, R_WIDTH, RA_LSB);
                ra = (word << (WORD_SIZE - (R_WIDTH + RA_LSB))) >> (WORD_SIZE - R_WIDTH); 

                // rb = Bitpack_getu(word, R_WIDTH, RB_LSB);
                rb = (word << (WORD_SIZE - (R_WIDTH + RB_LSB))) >> (WORD_SIZE - R_WIDTH); 

                // rc = Bitpack_getu(word, R_WIDTH, RC_LSB);
                rc = (word << (WORD_SIZE - (R_WIDTH + RC_LSB))) >> (WORD_SIZE - R_WIDTH); 

                /* Load Program */
                if (opcode == 12) {
                        /* Updates programs counter*/
                        prog_counter = load_program(mem, rb, rc);

                        // seg_zero = (UArray_T)Seq_get(mem->segments, 0);
                        seg_zero = (seg_arr)Seq_get(mem->segments, 0);
                        assert(seg_zero != NULL);

                        // seg_zero_len = UArray_length(seg_zero);
                        seg_zero_len = seg_arr_length(seg_zero);
                        } else {
                        instruction_call(mem, opcode, ra, rb, rc);
                }
        }
}