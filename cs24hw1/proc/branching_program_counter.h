/*! \file
 *
 * This file contains the declarations for the program counter of a branching
 * processor.  The program counter receives a signal from the branching unit
 * that tells it whether it should increment, or jump to a different location
 * in the program.
 */

#ifndef BRANCHING_PROGRAM_COUNTER_H
#define BRANCHING_PROGRAM_COUNTER_H


#include "bus.h"


/*!
 * This struct encapsulates the state for the branching program counter.
 * Besides the current program counter, the struct holds signals from the branch
 * unit, which tells the program counter when to branch, and if so, what
 * location to jump to.
 */
typedef struct ProgramCounter {
    /*! This is the program counter signal itself. */
    pin pc_pin;
    
    /*!
     * This signal tells the program counter when to branch.  It should be set
     * to BRANCH if the nextPC() function should jump to branch_addr, or it
     * should be set to NOBRANCH if nextPC() should just go to the next
     * instruction.
     */
    pin branch;
    
    /*!
     * When the branch pin tells the program counter to branch, this is the
     * address that the program counter jumps to.
     */
    pin branch_addr;
    
} ProgramCounter;



/* Documentation appears in branching_program_counter.c. */
ProgramCounter * build_program_counter();
void free_program_counter(ProgramCounter *pc);
void reset(ProgramCounter *pc);
void nextPC(ProgramCounter *pc);


#endif /* BRANCHING_PROGRAM_COUNTER_H */

