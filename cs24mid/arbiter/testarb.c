/*! \file
 *
 * This file contains some basic test code for exercising the bus arbiter.
 * Various operations are performed, and then the results are output to
 * the console for double-checking.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bus.h"
#include "arbiter.h"


/*! Entry-point for the bus-arbiter testing program. */
int main (int argc, char **argv) {
    pin req_a, req_b, grant_a, grant_b;
    BusArbiter *arb;
    int clock;

    /* If an argument is specified, treat it as the random number seed.
     * Otherwise, we will use the default seed.
     */
    if (argc == 2) {
        long seed = atol(argv[1]);
        printf("NOTE:  Using random seed %ld\n\n", seed);
        srand(seed);
    }


    /* Build up the bus arbiter and its buses. */

    arb = build_arbiter();

    connect(&req_a, &arb->req_a);
    connect(&req_b, &arb->req_b);

    connect(&grant_a, &arb->grant_a);
    connect(&grant_b, &arb->grant_b);

    /*
     * Run a simple random test where we set various request lines,
     * then clock the arbiter to see what it does.
     */

    clock = 0;
    do {
        if (clock >= 10) {
            /* Don't start tinkering with the request inputs until after a
             * certain amount of time, just so it's easier to understand
             * the program output.
             */
            int val = rand() % 100;

            /* Give each pin a 5% chance of toggling on each clock. */
            if (val < 5)
                pin_set(req_a, !pin_read(req_a));
            else if (val < 10)
                pin_set(req_b, !pin_read(req_b));
        }

        clock++;
        clock_arbiter(arb);

        printf("T = %03d     Turn = %lu   ReqA = %lu  ReqB = %lu  "
               "GrantA = %lu  GrantB = %lu\n",
               clock, arb->turn, pin_read(req_a), pin_read(req_b),
               pin_read(grant_a), pin_read(grant_b));

        /* Do some basic sanity checks. */

        if (pin_read(grant_a) == 1 && pin_read(grant_b)) {
            printf("\t\tERROR:  Arbiter has granted both processors"
                   " access to the bus!\n");
        }
    }
    while (clock < 100);

    printf("\nAll done!\n");
    free_arbiter(arb);

    return 0;
}

