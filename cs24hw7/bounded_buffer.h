/*
 * An implementation of a bounded buffer of integers.
 *
 *--------------------------------------------------------------------
 * Adapted from code for CS24 by Jason Hickey.
 * Copyright (C) 2003-2010, Caltech.  All rights reserved.
 */
#ifndef _BOUNDED_BUFFER_H
#define _BOUNDED_BUFFER_H


/*
 * The bounded buffer data type.
 */
typedef struct _bounded_buffer BoundedBuffer;

/*
 * The buffer holds elements of this type.
 */
typedef struct buffer_elem {
    int id;     /* Identifier of the producer */
    int arg;    /* Argument to the producer */
    int val;    /* Value produced */
} BufferElem;

/*
 * Create a new buffer with the specified maximum length.
 */
BoundedBuffer * new_bounded_buffer(int length);

/*
 * Add and remove values from the queue.
 */
void bounded_buffer_add(BoundedBuffer *bufp, const BufferElem *s);
void bounded_buffer_take(BoundedBuffer *bufp, BufferElem *s);

#endif /* _BOUNDED_BUFFER_H */

