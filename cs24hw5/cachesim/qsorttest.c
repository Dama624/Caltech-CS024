#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "memory.h"
#include "cache.h"
#include "cmdline.h"


#define NUM_ELEMS 1000000


/* Set to time(NULL) to generate new random data each time, or a constant to
 * generate the same random data each time.
 */
#define SEED 54321098


/* This helper handles the task of swapping two integers in the simulated
 * memory.
 */
void swap_values(membase_t *p_mem, int i, int j) {
    int i_val = read_int(p_mem, i);
    int j_val = read_int(p_mem, j);

    write_int(p_mem, i, j_val);
    write_int(p_mem, j, i_val);
}


/* This function partitions a range of values between the start and end
 * indexes, inclusive, and then returns the index of the pivot value.
 */
int partition(membase_t *p_mem, int start, int end) {
    int pivot_idx, pivot, swap_idx, i;

    assert(end > start);

    pivot_idx = (start + end) / 2;
    pivot = read_int(p_mem, pivot_idx);
    swap_values(p_mem, pivot_idx, end);

    swap_idx = start;
    for (i = start; i < end; i++) {
        if (read_int(p_mem, i) < pivot) {
            swap_values(p_mem, i, swap_idx);
            swap_idx++;
        }
    }

    swap_values(p_mem, swap_idx, end);

    return swap_idx;
}


/* This method implements the quicksort algorithm for a particular range
 * of values in the array.  The quicksort is implemented as a recursive
 * operation, and the array is sorted in-place.  The start and end indexes
 * are inclusive.
 */
void quicksort(membase_t *p_mem, int start, int end) {
    int pivot_idx;

    if (end <= start)
        return;

    pivot_idx = partition(p_mem, start, end);
    quicksort(p_mem, start, pivot_idx - 1);
    quicksort(p_mem, pivot_idx + 1, end);
}


/* This function is used by the C standard-library function qsort(), so that
 * we can check the output of our quicksort algorithm.
 */
int compare_int_ptrs(const void *v1, const void *v2) {
    int i1 = *(int *) v1;
    int i2 = *(int *) v2;

    return i1 - i2;
}


int main(int argc, const char **argv) {
    int *inputs;
    int i, error;
    membase_t *p_mem;

    /* Set up the simulated memory. */
    p_mem = make_cached_memory(argc, argv, NUM_ELEMS * sizeof(int));

    /* Generate random floats to sort. */

    printf("Generating %d random ints to sort.\n", NUM_ELEMS);

    inputs = malloc(NUM_ELEMS * sizeof(float));

    srand(SEED);

    /* Generate the inputs, then store them into the memory simulator in a
     * separate loop, so that the inputs we use don't change if the random
     * replacement policy is currently in effect.
     */
    for (i = 0; i < NUM_ELEMS; i++)
        inputs[i] = rand();
    
    for (i = 0; i < NUM_ELEMS; i++)
        write_int(p_mem, i, inputs[i]);

    /* Quicksort the array of integers. */

    printf("Quicksorting the array of integers.\n");
    quicksort(p_mem, 0, NUM_ELEMS - 1);

    /* Sort the inputs so that we can check the heap's results. */

    printf("Checking the results against the sorted inputs.\n");

    qsort(inputs, NUM_ELEMS, sizeof(float), compare_int_ptrs);

    error = 0;
    for (i = 0; i < NUM_ELEMS; i++) {
        int val = read_int(p_mem, i);
        if (val != inputs[i]) {
            printf("ERROR:  sorted arrays don't match at index %d!  "
                   "data[i] = %d, verify[i] = %d\n", i, val, inputs[i]);
            error = 1;
        }
    }

    if (error) {
        printf("Some values didn't match, aborting.\n");
        abort();
    }

    /* Print out the results of the quicksort. */

    printf("\nMemory-Access Statistics:\n\n");
    p_mem->print_stats(p_mem);
    printf("\n");

    return 0;
}
