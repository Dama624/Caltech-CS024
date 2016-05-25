/*
 * Provide C declarations for the functions in the glue code.
 *
 *--------------------------------------------------------------------
 * Adapted from code for CS24 by Jason Hickey.
 * Copyright (C) 2003-2010, Caltech.  All rights reserved.
 */
#ifndef _GLUE_H
#define _GLUE_H

/*
 * The structure of the thread's machine-context is opaque to the C code.
 * Thread structs reference the context by a ThreadContext *, i.e. a void *
 * (untyped pointer).
 */
typedef void ThreadContext;

/* The function that is passed to a thread must have this signature. */
typedef void (*ThreadFunction)(void *arg);

/*
 * Lock the scheduler.  This function is used to ensure
 * mutual exclusion in the scheduler.  The function returns
 * 1 if the lock was successful, otherwise it returns 0.
 */
extern int __sthread_lock(void);

/* Unlock the scheduler. */
extern void __sthread_unlock(void);

/* This function must be called to start thread processing. */
void __sthread_start(void);

/*
 * The is the entry point into the scheduler, to be called
 * whenever a thread action is required.
 */
void __sthread_schedule(void);

/*
 * Initialize the context for a new thread.
 *
 * The stackp pointer should point to the *end* of the area
 * allocated for the thread's stack.  (Don't forget that IA32
 * stacks grow downward in memory.)
 *
 * The function f is initially called with the argument given.
 * When f returns, __sthread_finish() is automatically called by
 * the threading library to ensure that the thread is cleaned up
 * and deallocated properly.
 */
ThreadContext *__sthread_initialize_context(void *stackp,
    ThreadFunction f, void *arg);

#endif /* _GLUE_H */

