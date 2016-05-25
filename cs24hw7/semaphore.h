/*
 * Semaphores for the sthread package.
 *
 *--------------------------------------------------------------------
 * Adapted from code for CS24 by Jason Hickey.
 * Copyright (C) 2003-2010, Caltech.  All rights reserved.
 */
#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

/*--------------------
 * Data-structure related information for semaphore
 *--------------------
 */
typedef struct _node Node;
typedef struct _list List;
int list_empty(List *listp);
void list_tail_append(List *listp, Thread *threadp);
Thread *list_take(List *listp);




/*
 * The semaphore's contents are opaque to users of the semaphore.  The
 * actual definition of the semaphore type is within semaphore.c.
 */
typedef struct _semaphore Semaphore;

/*
 * Create a new seamphore.
 * The argument is the initial value of the semaphore.
 */
Semaphore *new_semaphore(int init);

/*
 * Decrement the semaphore.
 * This operation will block if the semaphore value is zero.
 */
void semaphore_wait(Semaphore *semp);

/*
 * Increment the semaphore.
 */
void semaphore_signal(Semaphore *semp);

#endif /* _SEMAPHORE_H */

