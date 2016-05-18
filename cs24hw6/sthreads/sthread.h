/*!
 * Simple thread API.
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
#ifndef _STHREAD_H
#define _STHREAD_H


/*! Thread handles. */
typedef struct _thread Thread;


/*! The function that is passed to a thread must have this signature. */
typedef void (*ThreadFunction)(void *arg);


/*
 * Thread operations.
 */
Thread *sthread_create(ThreadFunction f, void *arg);
void sthread_yield(void);
void sthread_block(void);
void sthread_unblock(Thread *threadp);


/*
 * The start function should be called *once* in
 * the main() function of your program.  This function
 * never returns.
 */
void sthread_start(void);


#endif /* _STHREAD_H */

