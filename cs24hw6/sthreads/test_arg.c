#include <stdio.h>
#include "sthread.h"


/*
 * Both threads update the same global variable "global"
 * Thread 1 prints global + 1.
 * Thread 2 prints global - 1.
 *
 * In this version of the code,
 * thread scheduling is explicit.
 */

/*! This thread-function adds 1 to global and yields */
static void loop1(void *arg) {
    while(1) {
        *(int *)arg += 1;
        printf("Global = %d\n", *(int *)arg);
        sthread_yield();
    }
}

/*! This thread-function subtracts 1 to global and yields */
static void loop2(void *arg) {
    while(1) {
        *(int *)arg -= 1;
        printf("Global = %d\n", *(int *)arg);
        sthread_yield();
    }
}

/*
 * The main function starts the two loops in separate threads,
 * the start the thread scheduler.
 */
int main(int argc, char **argv) {
    int global = 5;

    sthread_create(loop1, (void *)&global);
    sthread_create(loop2, (void *)&global);
    sthread_start();
    return 0;
}


