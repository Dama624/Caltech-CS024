#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cmdline.h"
#include "heap.h"
#include "memory.h"
#include "cache.h"


#define NUM_ELEMS 1000000


/* Set to time(NULL) to generate new random data each time, or a constant to
 * generate the same random data each time.
 */
#define SEED 54321098


/* This function is used by the C standard-library function qsort(), so that
 * we can check the output of our heap-sort algorithm.
 */
int compare_float_ptrs(const void *v1, const void *v2) {
    float f1 = *(float *) v1;
    float f2 = *(float *) v2;

    if (f1 < f2)
        return -1;
    else if (f1 > f2)
        return 1;

    return 0;
}


int main(int argc, const char **argv) {
    float *inputs;
    int i, error;

    membase_t *p_mem;

    float_heap heap;

    /* Set up the simulated memory. */
    p_mem = make_cached_memory(argc, argv, NUM_ELEMS * sizeof(int));
    
    /* Generate random floats to sort. */

    printf("Generating %d random floats to sort.\n", NUM_ELEMS);

    inputs = malloc(NUM_ELEMS * sizeof(float));

    srand48(SEED);
    for (i = 0; i < NUM_ELEMS; i++)
        inputs[i] = (float) drand48();

    /* Use the heap to sort the sequence of floats. */

    printf("Sorting numbers using the heap.\n");

    init_heap(&heap, p_mem, NUM_ELEMS);
    for (i = 0; i < NUM_ELEMS; i++)
        add_value(&heap, inputs[i]);

    /* Sort the inputs so that we can check the heap's results. */

    printf("Checking the results against the sorted inputs.\n");

    qsort(inputs, NUM_ELEMS, sizeof(float), compare_float_ptrs);

    error = 0;
    for (i = 0; i < NUM_ELEMS; i++) {
        float val = get_first_value(&heap);
        if (val != inputs[i]) {
            printf("ERROR:  heap and sorted array don't match at "
                   "index %d!  heap = %f, val = %f\n", i, val, inputs[i]);
            error = 1;
        }
    }

    if (error) {
        printf("Some values didn't match, aborting.\n");
        abort();
    }

    /* Print out the results of the heap sort. */

    printf("\nMemory-Access Statistics:\n\n");
    p_mem->print_stats(p_mem);
    printf("\n");

    return 0;
}

