/*
 * Driver file for UM Implementation.
 * The driver opens the provided .um file,
 * creates a UM_T struct,
 * reads in all instructions from given file,
 * and populates segment zero with all instructions.
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <sys/stat.h>

// #include "um.h"
#include "memory.h"
#include "bitpack.h"

#define W_SIZE 32
#define CHAR_SIZE 8
#define CHAR_PER_WORD 4

void populate_seg_zero(Memory_T mem, FILE *fp, uint32_t size);
uint32_t construct_word(FILE *fp);

int main(int argc, char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "Usage: ./um <Um file>\n");
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[1], "r");
    assert(fp != NULL);

    struct stat file_info;
    stat(argv[1], &file_info);
    uint32_t size = file_info.st_size / CHAR_PER_WORD;

    Memory_T mem = memory_new(size);
    assert(mem != NULL);

    populate_seg_zero(mem, fp, size);

    fclose(fp);
    um_execute(mem);

    memory_free(mem);

    return EXIT_SUCCESS;
}

/* Name: populate_seg_zero
 * Input: A UM_t struct, a file pointer, and a size
 * Output: N/A
 * Does: Populates segment zero with words from the file
 * Error: Asserts if UM_T struct is NULL
 *        Asserts if fp does not exist
 */
void populate_seg_zero(Memory_T mem, FILE *fp, uint32_t size)
{
    assert(mem != NULL);
    assert(fp != NULL);

     uint32_t a = 0, b = 0, c = 0, d = 0, word = 0;
     
    for (uint32_t index = 0; index < size; ++index) {
        a = getc(fp); 
        b = getc(fp); 
        c = getc(fp); 
        d = getc(fp); 
        word = (a << 24) | (b << 16) | (c << 8) | d;
        memory_put(mem, 0, index, word);
        // mem->segments[0]->segment[index] = word; 
    }
}