/*! \file
 *
 * This file contains the definitions for the branch unit, the component that
 * determines when the processor should jump to a different program location.
 */

 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "branch_unit.h"
#include "instruction.h"


/*
 * Branch Unit (BRU)
 *
 *  The Branch Unit looks at the instruction
 *   and the selected src1 operand and determines
 *   when a branch should take place.
 *
 */


/*!
 * This function dynamically allocates and initializes the state for a new
 * branch-unit instance.  If allocation fails, the program is terminated.
 */
BranchUnit * build_branch_unit() {
    BranchUnit *bru = malloc(sizeof(BranchUnit));
    if (!bru) {
        fprintf(stderr, "Out of memory building a branch unit!\n");
        exit(11);
    }
    memset(bru, 0, sizeof(BranchUnit));
    return bru;
}


/*! This function frees the dynamically allocated branch-unit instance. */
void free_branch_unit(BranchUnit *bru) {
    free(bru);
}


/*! 
 * This function performs the branching test.  It reads the current opcode and
 * the output from the first source register, and then writes either BRANCH or
 * NOBRANCH to the branch pin.
 */
void branch_test(BranchUnit *bru) {
    uint32_t A;
    uint32_t aluop;

    A = pin_read(bru->in1);
    aluop = pin_read(bru->op);

    /*======================================================================*/
    /* TODO:  If aluop is BNZ, and A is nonzero, set bru->branch to BRANCH. */
    /*        Otherwise, set bru->branch to NOBRANCH.                       */
    /*                                                                      */
    /*        Use pin_set(bru->branch, ...) to set the value on the bus.    */
    /*======================================================================*/

}

