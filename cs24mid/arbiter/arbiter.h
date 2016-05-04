/*! \file
 *
 * This file contains declarations for a simple bus arbiter in
 * an emulated multiprocessor.
 */


#ifndef ARBITER_H
#define ARBITER_H


#include "bus.h"


/*!
 * This structure represents the data inputs, outputs, and internal
 * state for a bus arbiter.  The arbiter implementation (in arbiter.c)
 * takes the inputs and current state, and computes outputs and the
 * next state value, based on the description of how the arbiter
 * should control the shared bus.
 */
typedef struct BusArbiter {

   /* Internal State */
   unsigned long turn;  /*!< The internal value indicating whose turn it is. */

   /* Inputs */
   pin req_a;           /*!< Processor A's request input to the arbiter. */
   pin req_b;           /*!< Processor B's request input to the arbiter. */

   /* Outputs */
   pin grant_a;         /*!< The "grant access" signal to Processor A. */
   pin grant_b;         /*!< The "grant access" signal to Processor B. */

} BusArbiter;


/* Documentation appears in arbiter.c. */

BusArbiter * build_arbiter();
void free_arbiter(BusArbiter *arb);

void clock_arbiter(BusArbiter *arb);


#endif /* ARBITER_H */

