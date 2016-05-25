/*
 * This module adds a timer interrupt to time-slice the threads.  The
 * details of operation of this file are not important for understanding
 * HW7.  The end result is to ensure that __sthread_schedule is called
 * periodically.
 *
 * The principle is as follows:
 *
 * 1.  Set up a periodic timer interrupt (the period is specified
 *     by the QUANTUM_* constants below).
 *
 * 2.  In the signal handler for SIGALRM, set up the return context
 *     so that the signal handler returns to __sthread_schedule,
 *     instead than the point where the exception occurred.  The
 *     return address is saved on the stack.
 *
 *--------------------------------------------------------------------
 * Adapted from code for CS24 by Jason Hickey.
 * Copyright (C) 2004-2010, Caltech.  All rights reserved.
 */

#define __USE_GNU
#define _GNU_SOURCE
#include <ucontext.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <memory.h>
#include <sys/time.h>

#include "timer.h"
#include "glue.h"

/*
 * Default timeslice quantum.  This is configured for a 10ms timeslice;
 * unfortunately, this tends to be just about the smallest resolution
 * for a lot of platforms, so we don't get to switch between threads
 * as much as we might like to.
 */
#define QUANTUM_SEC     0
#define QUANTUM_USEC    10000

/*
 * Default size of signal stack is set to 64k.
 */
#define SIGNAL_STACKSIZE        (1 << 16)

/*
 * When the timer fires, set up the process context so that
 * the signal handler returns to the __sthread_schedule function.
 */
static void timer_action(int signum, siginfo_t *infop, void *data) {
    /*
     * Get the scheduler lock.
     * If the scheduler is already active, ignore this timer interrupt.
     */
    if (__sthread_lock()) {
        ucontext_t *contextp = (ucontext_t *) data;
        greg_t *regs = contextp->uc_mcontext.gregs;
        void **esp = (void **) regs[REG_ESP];
        void *eip = (void *) regs[REG_EIP];

        /* Push the current program counter as the new return address */
        *--esp = eip;

        /* Set the program counter to the __sthread_schedule function */
        eip = (void *) __sthread_schedule;

        /* Save these two registers back to the context */
        regs[REG_ESP] = (greg_t) esp;
        regs[REG_EIP] = (greg_t) eip;
    }
}

/*
 * Start the timer to generate periodic interrupts.
 * Signals are handled on an alternate stack, so that
 * we can manipulate the process stack in the signal
 * handler.
 */
void start_timer() {
    static char sigstack[SIGNAL_STACKSIZE];
    struct sigaction action;
    struct itimerval itimer;
    stack_t stackinfo;

    /* Handle signals on an alternate stack */
    stackinfo.ss_sp = sigstack;
    stackinfo.ss_flags = SS_ONSTACK;
    stackinfo.ss_size = sizeof(sigstack);
    if (sigaltstack(&stackinfo, (stack_t *) 0) < 0) {
        perror("sigaltstack");
        exit(1);
    }

    /* Install the signal handler */
    memset(&action, 0, sizeof(action));
    action.sa_sigaction = timer_action;
    action.sa_flags = SA_SIGINFO | SA_ONSTACK;
    if (sigaction(SIGALRM, &action, (struct sigaction *) 0) < 0) {
        perror("sigaction");
        exit(1);
    }

    /* Start the periodic timer */
    itimer.it_interval.tv_sec = QUANTUM_SEC;
    itimer.it_interval.tv_usec = QUANTUM_USEC;
    itimer.it_value.tv_sec = QUANTUM_SEC;
    itimer.it_value.tv_usec = QUANTUM_USEC;
    if (setitimer(ITIMER_REAL, &itimer, (struct itimerval *) 0) < 0) {
        perror("setitimer");
        exit(1);
    }
}

