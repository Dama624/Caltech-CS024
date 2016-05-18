/*!
 * A simple test program for exercising the threads API.
 *
 * ----------------------------------------------------------------
 *
 * @begin[license]
 * Copyright (C) 2003 Jason Hickey, Caltech
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Author: Jason Hickey
 * @end[license]
 */
#include <stdio.h>
#include "sthread.h"


/*
 * Thread 1 prints "Hello".
 * Thread 2 prints "Goodbye".
 *
 * In this version of the code,
 * thread scheduling is explicit.
 */

/*! This thread-function prints "Hello" over and over again! */
static void loop1(void *arg) {
    while(1) {
        printf("Hello\n");
        sthread_yield();
    }
}

/*! This thread-function prints "Goodbye" over and over again! */
static void loop2(void *arg) {
    while(1) {
        printf("Goodbye\n");
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
    sthread_start();
    return 0;
}


