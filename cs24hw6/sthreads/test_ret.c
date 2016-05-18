#include <stdio.h>
#include "sthread.h"


/*
 * 4 threads each run a for-loop that runs for a different length of time
 * Thread 1 runs for 1 iterations.
 * Thread 2 runs for 2 iterations.
 * Thread 3 runs for 3 iterations.
 * Thread 4 runs for 4 iterations.
 *
 * In this version of the code,
 * thread scheduling is explicit.
 */

/*! This thread-function runs for 1 iteration */
static void loop1(void *arg) {
    int i;
    for (i = 0; i < 1; i++) {
        printf("Thread 1 was here!\n");
        sthread_yield();
    }
}

/*! This thread-function runs for 2 iterations */
static void loop2(void *arg) {
    int i;
    for (i = 0; i < 2; i++) {
        printf("Thread 2 was here!\n");
        sthread_yield();
    }
}

/*! This thread-function runs for 3 iterations */
static void loop3(void *arg) {
    int i;
    for (i = 0; i < 3; i++) {
        printf("Thread 3 was here!\n");
        sthread_yield();
    }
}

/*! This thread-function runs for 4 iterations */
static void loop4(void *arg) {
    int i;
    for (i = 0; i < 4; i++) {
        printf("Thread 4 was here!\n");
        sthread_yield();
    }
}

/*
 * The main function starts the two loops in separate threads,
 * the start the thread scheduler.
 */
int main(int argc, char **argv) {

    sthread_create(loop1, NULL);
    sthread_create(loop2, NULL);
    sthread_create(loop3, NULL);
    sthread_create(loop4, NULL);
    sthread_start();
    return 0;
}


