/*! \file
 *
 * This file contains declarations for the branch unit, the component that
 * determines when the processor should jump to a different program location.
 */


#ifndef BRANCH_UNIT_H
#define BRANCH_UNIT_H


#include "bus.h"


/*!
 * Branch Unit (BRU)
 *
 * The Branch Unit looks at the instruction and the selected src1 operand and
 * determines when a branch should take place.
 */
typedef struct BranchUnit {
    /* Inputs */
    
    /*!
     * This pin allows the branch unit to read the output of thie first source
     * register, so that the unit can determine if it is time to jump.
     */
    pin in1;
    
    /*!
     * This pin allows the branch unit to read the opcode of the current
     * instruction, so that it can determine if it's time to jump.
     */
    pin op;

    /* Outputs */
    
    /*!
     * This output allows the branch unit to tell the program counter when it
     * is time to jump.  The test() function sets this pin to the value BRANCH
     * when the program counter should branch.
     */
    pin branch;
    
} BranchUnit;


/* Documentation appears in branch_unit.c. */
BranchUnit * build_branch_unit();
void free_branch_unit(BranchUnit *bru);
void branch_test(BranchUnit *bru);


#endif /* BRANCH_UNIT_H */

