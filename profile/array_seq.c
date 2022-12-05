#include "array_seq.c"
#include <stdlib.h>
#include <stdio.h>


/* uint arr */
struct seg_arr {
        uint32_t *segment;
        int length;
}

/* uarray new */

seg_arr seg_arr_new(int length) {
        /* Mallocs a uarray */
        seg_arr seg = malloc(sizeof(*seg));
        /* */
        seg->segment = malloc(sizeof(uint32_t) * length);
        seg->length = length;
        return seg;
}

/* uarray at */
void seg_arr_add(seg_arr seg, uint32_t word, int index) {
        seg->segment[index] = word;
}


/* uarray length */
int seg_arr_length(seg_arr seg) {
        return seg->length;
}

/* free */
void seg_arr_free(seg_arr seg) {
        free(seg->segment); 
        free(seg);
}




/* seq get */


/* seq put */

/* add hi */

/* add low */

/* rem hi */

/* rem low */