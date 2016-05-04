/*! \file
 *
 * This file contains definitions for simple buses used in an emulated
 * processor.
 *
 * Conceptually, a "bus" is a set of wires that communicates a value from a
 * producer to one or more consumers.  A "pin" is an endpoint of a bus.  The
 * connect() functions create a bus and then connect pins to the bus.
 *
 * In the implementation, a bus is actually represented as a variable in memory
 * (in this case, allocated on the heap).  The busdata_t typedef is the data
 * type used for buses.  A "pin" is then simply a variable of type busdata_t*,
 * that holds the address of the bus variable, and therefore can read or write
 * the bus variable.  This means that multiple pins can be connected to a single
 * bus variable, and they can communicate values back and forth by reading and
 * writing to the bus variable.
 */

 
#include <stdio.h>
#include <stdlib.h>  /* malloc(), free() */

#include "bus.h"


/*!
 * This function connects two pins together by creating a "bus" between them,
 * and then returns the newly allocated bus.  Since pins are addresses of an
 * unsigned long that can be used to send signals between the pins, this
 * function simply allocates the unsigned long (
 */
bus connect(pin *pin1, pin *pin2) {
    bus res = malloc(sizeof(busdata_t));
    if (!res) {
        fprintf(stderr, "Out of memory allocating bus.");
        exit(11);
    }

    // Connect the "pins" to the "bus".
    *pin1 = *pin2 = res;

    return res;
}


/*! This function frees the dynamically allocated bus instance. */
void free_bus(bus b) {
    free(b);
}


/*!
 * This function connects three pins together so that writing to any of them
 * will modify all of them.  It is identical to the two-input connect()
 * function in that it allocates a bus, sets all pins to refer to the bus, then
 * returns the newly allocated bus.
 */
bus connect3(pin *pin1, pin *pin2, pin *pin3) {
    bus res = connect(pin1, pin2);
    *pin3 = res;
    return res;
}


