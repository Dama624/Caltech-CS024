/*
 * Define a bounded buffer containing records that describe the
 * results in a producer thread.
 *
 *--------------------------------------------------------------------
 * Adapted from code for CS24 by Jason Hickey.
 * Copyright (C) 2003-2010, Caltech.  All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include "sthread.h"
#include "bounded_buffer.h"
#include "semaphore.h"

/*
 * The bounded buffer data.
 */
struct _bounded_buffer {
    /* The maximum number of elements in the buffer */
    int length;

    /* The index of the first element in the buffer */
    int first;

    /* The number of elements currently stored in the buffer */
    int count;

    /* The semaphores for the buffer:
     * avail keeps track of how much space is available
     * used keeps track of how much space is used
     * each unit "space" refers to a single element*/
    Semaphore *avail;
    Semaphore *used;

    /* The values in the buffer */
    BufferElem *buffer;
};

/*
 * For debugging, ensure that empty slots in the buffer are
 * set to a default value.
 */
static BufferElem empty = { -1, -1, -1 };

/*
 * Allocate a new bounded buffer.
 */
BoundedBuffer *new_bounded_buffer(int length) {
    BoundedBuffer *bufp;
    BufferElem *buffer;
    Semaphore *avail;
    Semaphore *used;
    int i;

    /* Allocate the buffer */
    buffer = (BufferElem *) malloc(length * sizeof(BufferElem));
    bufp = (BoundedBuffer *) malloc(sizeof(BoundedBuffer));
    avail = new_semaphore(length);
    used = new_semaphore(0);
    if (buffer == 0 || bufp == 0) {
        fprintf(stderr, "new_bounded_buffer: out of memory\n");
        exit(1);
    }

    /* Initialize */

    memset(bufp, 0, sizeof(BoundedBuffer));

    for (i = 0; i != length; i++)
        buffer[i] = empty;

    bufp->length = length;
    bufp->avail = avail;
    bufp->used = used;
    bufp->buffer = buffer;

    return bufp;
}

/*
 * Add an integer to the buffer.  Yield control to another
 * thread if the buffer is full.
 */
void bounded_buffer_add(BoundedBuffer *bufp, const BufferElem *elem) {
    // We only want one thread modifying the buffer at a time
    __sthread_lock();
    // Decrement the count of available space (we're adding)
    // Blocks when available count = 0 (buffer is full)
    semaphore_wait(bufp->avail);
    bufp->buffer[(bufp->first + bufp->count) % bufp->length] = *elem;
    bufp->count++;
    // Increment the count of used space
    semaphore_signal(bufp->used);
    __sthread_unlock();
}

/*
 * Get an integer from the buffer.  Yield control to another
 * thread if the buffer is empty.
 */
void bounded_buffer_take(BoundedBuffer *bufp, BufferElem *elem) {
    // We only want one thread modifying the buffer at a time
    __sthread_lock();
    // Decrement count of used space (we're freeing)
    // Blocks when there are no used elements (buffer is empty)
    semaphore_wait(bufp->used);
    /* Copy the element from the buffer, and clear the record */
    *elem = bufp->buffer[bufp->first];
    bufp->buffer[bufp->first] = empty;
    bufp->count--;
    bufp->first = (bufp->first + 1) % bufp->length;
    // Increment count of available space
    semaphore_signal(bufp->avail);
    __sthread_unlock();
}
