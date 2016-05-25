/*
 * This module adds a periodic timer to ensure that the
 * __sthread_schedule function is called periodically.
 *
 *--------------------------------------------------------------------
 * Adapted from code for CS24 by Jason Hickey.
 * Copyright (C) 2004-2010, Caltech.  All rights reserved.
 */
#ifndef _TIMER_H
#define _TIMER_H

/*
 * Start the periodic timer.
 */
void start_timer(void);

#endif /* _TIMER_H */

