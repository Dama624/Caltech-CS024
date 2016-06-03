#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <time.h>

#include "accum.h"


/* Number of threads to use for adding to the accumulator. */
#define NUM_THREADS 4

/* Number of values that each thread will try to add to the accumulator. */
#define NUM_VALUES 50000000


/* The accumulator that we will sum values into. */
static uint32_t accum;


/* A simple helper function that we use to do very basic testing on the
 * accumulator before running the multithreaded test, so that we can at
 * least know the accumulator works...
 */
int try_add_to_accum(uint32_t value) {
    uint32_t initial = accum;
    uint32_t result = add_to_accum(&accum, value);
    int success = 1;

    if (result != initial + value) {
        printf("  ERROR:  add_to_accum(&accum, %u) was expected to return %u,"
               " got %u instead\n", value, initial + value, result);
        success = 0;
    }

    if (accum != initial + value) {
        printf("  ERROR:  After add_to_accum, %u), expected accum to be %u,"
               " was %u instead\n", value, initial + value, accum);
        success = 0;
    }

    return success;
}


/* A simple thread-function that adds values to the accumulator.  We do this
 * from threads so that we can identify concurrency/parallelism bugs.
 */
void * thread_func(void *arg) {
    int i;
    uint32_t *values = (uint32_t *) arg;

    for (i = 0; i < NUM_VALUES; i++)
        add_to_accum(&accum, values[i]);

    return NULL;
}


/* Tests the accumulator, first with a few very basic tests, and then from
 * multiple threads so that we can identify any possible concurrency issues.
 */
int main() {
    pthread_t *threads;
    uint32_t **values;
    uint32_t sum;
    int i, j, rc;

    /*-------------------------------------------------------------------------
     * First do a simple test to make sure the accumulator works at all.
     */

    printf("Performing simple test on accumulator.\n");
    accum = 0;
    if (!try_add_to_accum(1))
        return 1;
    if (!try_add_to_accum(3))
        return 1;
    if (!try_add_to_accum(5))
        return 1;
    printf("Accumulator seems to work in single-threaded use.\n\n");

    /*-------------------------------------------------------------------------
     * Next, do the test with concurrent additions to the accumulator.
     */

    printf("Performing concurrency test on accumulator with %d threads,\n"
           "each adding %d values.\n\n", NUM_THREADS, NUM_VALUES);

    srand(time(NULL));

    /* Initialize the accumulator to 0. */
    accum = 0;

    /* Create the arrays of values that the threads will use to sum into the
     * accumulator.  Keep track of the overall sum so that we can check the
     * final result.
     */
    values = malloc(NUM_THREADS * sizeof(uint32_t *));
    sum = 0;
    for (i = 0; i < NUM_THREADS; i++) {
        values[i] = malloc(NUM_VALUES * sizeof(uint32_t));
        for (j = 0; j < NUM_VALUES; j++) {
            values[i][j] = rand() % 20 + 1;
            sum += values[i][j];
        }
    }

    /* Create all the threads now.  Ideally we would use a synchronization
     * barrier to make sure all the threads start at the same time, but we
     * will just hope that threads will have enough work to do that they
     * have a lot of overlap with each other.
     */
    threads = malloc(NUM_THREADS * sizeof(pthread_t));
    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_create(threads + i, NULL, thread_func, values[i]);
        if (rc != 0) {
            printf("ERROR:  Couldn't create thread %d!\n", i);
            exit(1);
        }
    }

    /* Wait for all threads to finish. */
    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc != 0)
            printf("WARNING:  Couldn't join thread %d\n", i);
    }

    /* Does the actual sum match the expected one? */
    if (sum == accum) {
        printf("SUCCESS:  The accumulator holds the expected sum of %u.\n",
               sum);
    }
    else {
        printf("ERROR:  Accumulator holds %u, but the expected sum is %u\n",
               accum, sum);
    }

    /* Free stuff */
    for (i = 0; i < NUM_THREADS; i++)
        free(values[i]);

    free(values);
    free(threads);

    return 0;
}

