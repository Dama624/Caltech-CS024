/*
 * Test program implements a producer-consumer problem.
 * There are two producers that each produce Fibonacci
 * numbers and add the results to the buffer.  There
 * is a single consumer that takes the results from
 * the buffer, checks that the values are correct,
 * and prints them out.
 *
 *--------------------------------------------------------------------
 * Adapted from code for CS24 by Jason Hickey.
 * Copyright (C) 2003-2010, Caltech.  All rights reserved.
 */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include "sthread.h"
#include "semaphore.h"
#include "bounded_buffer.h"

/*
 * Queue for passing values between the producers and the consumer.
 *
 * Don't mess with the buffer-length; it is tuned to expose the concurrency
 * issues you will focus on in the assignment.
 */

#define DEFAULT_BUFFER_LENGTH   10

static BoundedBuffer *queue;


/*
 * Recursive Fibonacci.
 */
static int fib(int i) {
    if (i < 2)
        return 1;

    return fib(i - 1) + fib(i - 2);
}

/*
 * Producer produces Fibonacci numbers mod FIB_MODULUS.
 */

#define FIB_MODULUS     20

static void producer(void *arg) {
    BufferElem elem;
    int i = 0;

    elem.id = (int) arg;
    while (1) {
        /* Place the next computed Fibonacci result into the buffer */
        elem.arg = i;
        elem.val = fib(i);
        bounded_buffer_add(queue, &elem);
        i = (i + 1) % FIB_MODULUS;
    }
}


/*
 * Consumer prints them out.
 */
static void consumer(void *arg) {
    BufferElem elem;
    int i;

    /* Read the contents of the buffer, and print them */
    while (1) {
        bounded_buffer_take(queue, &elem);
        i = fib(elem.arg);
        printf("Result from producer %d: Fib %2d = %6d; should be %6d; %s\n",
               elem.id, elem.arg, elem.val, i,
               i == elem.val ? "matched" : "NO MATCH");
    }
}


/*
 * The main function starts the two producers and the consumer,
 * the starts the thread scheduler.
 */
int main(int argc, char **argv) {
    queue = new_bounded_buffer(DEFAULT_BUFFER_LENGTH);
    sthread_create(producer, (void *) 0);
    sthread_create(producer, (void *) 1);
    sthread_create(consumer, (void *) 0);

    /*
     * Start the thread scheduler.  By default, the timer is
     * not started.  Change the argument to 1 to start the timer.
     */
    sthread_start(1);
    return 0;
}

