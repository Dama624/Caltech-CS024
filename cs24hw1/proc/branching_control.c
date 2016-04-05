/*! \file
 *
 * This file contains the controller for the branching processor.  It
 * coordinates the operations that must occur each time a clock occurs in the
 * system.
 */
 

#include <stdio.h>

#include "branching_control.h"


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

static int VERBOSE = 0;


/*!
 * This function performs the operations of the processor for each tick of the
 * clock.  This includes fetching and decoding the current instruction, reading
 * the register file, activating both the branch unit and the ALU, writing any
 * output back to the register file, then advancing to the next instruction.
 */
void clock(Processor* proc) {
    ifetch(proc->is);
    idecode(proc->decode);
    rf_read(proc->rf);
    branch_test(proc->bru);
    alu_eval(proc->alu);
    rf_write(proc->rf);
    if (VERBOSE > 0)
        rfprint(stdout, proc->rf);
    nextPC(proc->pc);
}
