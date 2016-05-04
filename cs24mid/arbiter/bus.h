/*! \file
 *
 * This file contains declarations for simple buses used in an emulated
 * processor.
 *
 * A bus is a set of wires that communicates a value from a producer to one or
 * more consumers.
 *
 * A pin is an endpoint of a bus.  The connect() function creates a bus and
 * connects both pins to it.
 *
 * Here, we represent a bus as a location in memory.
 */


#ifndef BUS_H
#define BUS_H


/* bus
 *
 *  A bus is a set of wires that communicates
 *  a value from a producer to one or more
 *  consumers.
 *
 *  A pin is an endpoint of a bus.  connect()
 *  creates a bus and connects both pins to it.
 *
 *
 *  Here, we represent a bus as a location in memory.
 *
 *  You cannot use multiple connections to connect
 *    more than two things together.
 *  WRONG:  connect(a,b); connect(b,c);
 *  instead use connect3(a,b,c);
 *  If you need to connect more than 3 things, you'll
 *    need to modify bus.{c,h} to create a suitably
 *    larger connectN().
 *
 */


/*!
 * This is the actual datatype of the bus data itself.  Both buses and pins are
 * pointers to values of this type.
 */
typedef unsigned long int busdata_t;


/*!
 * A bus is a set of wires that communicates a value from a producer to one or
 * more consumers.
 */
typedef busdata_t *bus;


/*!
 * A pin is an endpoint of a bus.  connect() creates a bus and connects both
 * pins to it.
 */
typedef busdata_t *pin;


/*!
 * This C macro function sets a bus to a specific value, by dereferencing the
 * pin (the address of the bus) and then assigning a value to it.
 */
#define pin_set(pin, value) *(pin) = (value)

/*!
 * This C macro function reads the current value of a bus, by dereferencing the
 * pin (the address of the bus) to get the value.
 */
#define pin_read(pin) *(pin)

/*!
 * This C macro function sets a bus to a specific value, by dereferencing the
 * bus (an address of a memory location) and then assigning a value to it.
 */
#define bus_set(bus, value) pin_set((bus), (value))

/*!
 * This C macro function reads the current value of a bus, by dereferencing the
 * bus (an address of a memory location) to get the value.
 */
#define bus_read(bus) pin_read(bus)


/* Documentation appears in bus.c. */
bus connect(pin *bus1, pin *bus2);
bus connect3(pin *bus1, pin *bus2, pin *bus3);
void free_bus(bus);


#endif /* BUS_H */

