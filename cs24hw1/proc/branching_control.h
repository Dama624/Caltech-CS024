/*! \file
 *
 * This file contains declarations for the controller of the branching
 * processor.  It coordinates the operations that must occur each time a clock
 * occurs in the system.
 */

 
#ifndef BRANCHING_CONTROL_H
#define BRANCHING_CONTROL_H


#include "branching_processor.h"


/*
 * Branching Control
 *
 *  The Control Unit is responsible for
 *   sequencing the components of the CPU.
 *
 *
 *  This controller is a simplified, central coordinator
 *    that properly models the way the units should interact.
 *  In a synchronous processor, clocked registers and combinational 
 *    dataflow between the units would serve this same role.
 *  In an asynchronous process, the units would handshake with
 *    each on the channels (busses) which interconnect them.
 *
 */


void clock(Processor *proc);


#endif /* BRANCHING_CONTROL_H */

