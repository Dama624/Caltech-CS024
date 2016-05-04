/*! \file
 *
 * This file contains definitions for a simple bus arbiter in
 * an emulated multiprocessor.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>   /* malloc(), free() */
#include <string.h>   /* memset() */

#include "arbiter.h"


/*!
 * This function dynamically allocates and initializes the state for a new
 * bus-arbiter instance.  If allocation fails, the program is terminated.
 */
BusArbiter * build_arbiter() {
    /* Try to allocate the BusArbiter struct.  If this fails,
     * report error then exit.
     */
    BusArbiter *arb = malloc(sizeof(BusArbiter));
    if (!arb) {
        fprintf(stderr, "Out of memory building an arbiter!\n");
        exit(11);
    }

    /* Initialize all values in the BusArbiter struct to 0. */
    memset(arb, 0, sizeof(BusArbiter));
    return arb;
}


/*! This function frees the dynamically allocated BusArbiter instance. */
void free_arbiter(BusArbiter *arb) {
    free(arb);
}


/*!
 * This function implements the logic of the bus arbiter.  Based on the
 * inputs and the arbiter's internal state, the outputs are updated
 * accordingly.  The important rules are:
 *
 *   - AT MOST ONE grant signal may be 1.  All other grant signals
 *     must be 0.
 *
 *   - The grant signal whose value is 1 must correspond to the processor
 *     whose turn it currently is.
 *
 *   - As long as no processors are requesting the bus, the Turn value
 *     should toggle between 0 (processor A) and 1 (processor B).  Otherwise,
 *     the Turn value should remain set on the processor that is currently
 *     granted access to the bus.  (See previous point.)
 */
void clock_arbiter(BusArbiter *arb) {
    /* Current state and inputs. */
    unsigned long CurrentTurn;
    unsigned long ReqA, ReqB;

    /* Outputs and next state. */
    unsigned long GrantA, GrantB;
    unsigned long NextTurn;

    /* Bring in the current state and inputs, for the logic to use. */

    ReqA = pin_read(arb->req_a);
    ReqB = pin_read(arb->req_b);
    CurrentTurn = arb->turn;

    assert(ReqA == 0 || ReqA == 1);
    assert(ReqB == 0 || ReqB == 1);
    assert(CurrentTurn == 0 || CurrentTurn == 1);


    /*==============================================*/
    /* Implement the bus-arbiter logic here.        */
    /*==============================================*/
    // No precessor is requesting the bus
    if (ReqA == 0 && ReqB == 0)
    {
    	// The Turn value is toggled between 0 and 1
    	NextTurn = 1 - CurrentTurn;
    	// All Grant outputs are 0
    	GrantA = 0;
    	GrantB = 0;
    }
	else if (ReqA == 1 && ReqB == 1) // Both processors are requesting
	{
		NextTurn = CurrentTurn;
		if (CurrentTurn == 1) // Current Turn is Processor B
		{
			// Grant Process B
			GrantA = 0;
			GrantB = 1;
		}
		else // Current Turn is Processor A
		{
			// Grant Process A
			GrantA = 1;
			GrantB = 0;
		}
	}
	else if (ReqA == 1 && ReqB == 0)
	{
		if (CurrentTurn == 1)
		{
			// Alternate Turn if B is not requesting but A is, and Turn is B
			NextTurn = 1 - CurrentTurn;
			// Turn is B; Processor B has opportunity to access, even if it
			// doesn't need to
			GrantA = 0;
			GrantB = 1;
		}
		else
		{
			// Turn is A & A is requesting. Maintain turn.
			NextTurn = CurrentTurn;
			// Grant Process A.
			GrantA = 1;
			GrantB = 0;
		}
	}
	else // A is not requesting but B is
	{
		if (CurrentTurn == 1)
		{
			NextTurn = CurrentTurn;
			GrantA = 0;
			GrantB = 1;
		}
		else
		{
			NextTurn = 1 - CurrentTurn;
			GrantA = 1;
			GrantB = 0;
		}
	}

    /* Set outputs and update the state! */

    assert(GrantA == 0 || GrantA == 1);
    assert(GrantB == 0 || GrantB == 1);
    assert(NextTurn == 0 || NextTurn == 1);

    pin_set(arb->grant_a, GrantA);
    pin_set(arb->grant_b, GrantB);
    arb->turn = NextTurn;
}

