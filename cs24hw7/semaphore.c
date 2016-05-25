/*
 * General implementation of semaphores.
 *
 *--------------------------------------------------------------------
 * Adapted from code for CS24 by Jason Hickey.
 * Copyright (C) 2003-2010, Caltech.  All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

#include "sthread.h"
#include "semaphore.h"

/*
 * A node to be used to construct the list that keeps track of the blocked
 * threads
 */
struct _node {
    Thread *thread;
    Node *next;
};

/*
 * The list has a pointer to the head and the last element. We use the list
 * to keep track of the order of threads that have been blocked in
 * semaphore_wait().
 */
struct _list {
    Node *head;
    Node *tail;
};

/*
 * Returns true (1) if the specified list is empty.  Otherwise, returns
 * false (0).
 */
int list_empty(List *listp) {
    assert(listp != NULL);
    return (listp->head == NULL);
}

/*
 * Add a thread to the tail of the list.
 * If the list is empty, add it to the head.
 */
void list_tail_append(List *listp, Thread *threadp) {
    assert(listp != NULL);
    assert(threadp != NULL);

    Node *newnode = (Node *)malloc(sizeof(Node));
    // Assert that malloc succeeded
    if (newnode == NULL) {
        fprintf(stderr, "Can't allocate a node for semaphore list\n");
        exit(1);
    }

    newnode->thread = threadp;
    newnode->next = NULL;
    if(listp->head == NULL) {
        listp->head = newnode;
        listp->tail = newnode;
    }
    else {
        listp->tail->next = newnode;
        listp->tail = newnode;
    }
}

/*
 * Get the first process from the list of blocked threads.
 */
Thread *list_take(List *listp) {
    Node *nodep;
    Thread *threadp;

    assert(listp != NULL);

    /* Return NULL if the list is empty */
    if(listp->head == NULL)
        return NULL;

    /* Otherwise pop off the first thread */
    nodep = listp->head;
    threadp = listp->head->thread;
    // Only one element in list
    if(nodep == listp->tail) {
        listp->head = NULL;
        listp->tail = NULL;
    }
    else {
        listp->head = listp->head->next;
    }
    free(nodep);
    return threadp;
}

/*
 * The semaphore data structure contains an integer i, which represents our
 * semaphore value, and a list that keeps track of blocked threads
 */
struct _semaphore {
    int i;
    List *blocked;
};

/************************************************************************
 * Top-level semaphore implementation.
 */

/*
 * Allocate a new semaphore.  The initial value of the semaphore is
 * specified by the argument.
 */
Semaphore *new_semaphore(int init) {
    Semaphore *semp;
    List *blocked;

    /* First allocate space for a semaphore */
    blocked = (List *)malloc(sizeof(List));
    semp = (Semaphore *)malloc(sizeof(Semaphore));
    // Assert that malloc succeeded
    if (semp == NULL || blocked == NULL) {
        fprintf(stderr, "Can't allocate a semaphore\n");
        exit(1);
    }

    /* Initialize the newly allocated semaphore */
    blocked->head = NULL;
    blocked->tail = NULL;
    semp->i = init;
    semp->blocked = blocked;
    return semp;
}

/*
 * Decrement the semaphore.
 * This operation must be atomic, and it blocks iff the semaphore is zero.
 */
void semaphore_wait(Semaphore *semp) {

    // We must lock the thread to ensure wait is executed atomically
    __sthread_lock();
    // Semaphore is zero; we must block the thread and wait until the
    // semaphore is incremented
    while (semp->i == 0)
    {
        // add thread to list
        list_tail_append(semp->blocked, sthread_current());
        // block thread
        sthread_block(1);
        // once thread is unblocked, it is also unlocked
        // relock the thread before decrementing semaphore
        __sthread_lock();
    }
    semp->i--;
    // We safely decremented the semaphore, we can now unlock the thread
    __sthread_unlock();
}

/*
 * Increment the semaphore.
 * This operation must be atomic.
 */
void semaphore_signal(Semaphore *semp) {
    // We must lock the thread before the operation to enforce that it is run
    // atomically
    __sthread_lock();
    semp->i++;
    // If a thread is blocked, unblock it
    if (semp->blocked->head != NULL)
    {
        sthread_unblock(list_take(semp->blocked));
    }
    // We safely incremented the semaphore, we can now unlock the thread
    __sthread_unlock();
}

