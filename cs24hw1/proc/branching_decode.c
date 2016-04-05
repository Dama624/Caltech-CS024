/*! \file
 *
 * This file contains the definitions for the instruction decoder for the
 * branching processor.  The instruction decoder extracts bits from the current
 * instruction and turns them into separate fields that go to various functional
 * units in the processor.
 */

 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "branching_decode.h"
#include "instruction.h"


/*
 * Branching Instruction Decoder
 *
 *  The Instruction Decoder extracts
 *   bits from the instruction and
 *   turn them into separate fields that
 *   go to various functional units in the
 *   processor.
 *
 */

 
/*!
 * This function dynamically allocates and initializes the state for a new 
 * branching instruction decoder instance.  If allocation fails, the program is
 * terminated.
 */
Decode * build_decode() {
    Decode *d = malloc(sizeof(Decode));
    if (!d) {
        fprintf(stderr, "Out of memory building an instruction decoder!\n");
        exit(11);
    }
    memset(d, 0, sizeof(Decode));
    return d;
}


/*!
 * This function frees the dynamically allocated instruction-decoder instance.
 */
void free_decode(Decode *d) {
    free(d);
}


/*!
 * This function decodes the instruction on the input pin, and writes all of the
 * various components to output pins.  Other components can then read their
 * respective parts of the instruction.
 */
void idecode(Decode *d) {
    busdata_t instr = pin_read(d->input);

    pin_set(d->aluop,       ((instr >> ALUOP_POS) & ALUOP_MASK));
    pin_set(d->write,       ((instr >> WRITE_POS) & WRITE_MASK));
    pin_set(d->src1_addr,   ((instr >> SRC1_POS)  & SRC1_MASK));
    pin_set(d->src2_addr,   ((instr >> SRC2_POS)  & SRC2_MASK));
    pin_set(d->dst_addr,    ((instr >> DST_POS)   & DST_MASK));
    pin_set(d->branch_addr, ((instr >> BRANCH_ADDR_POS) & BRANCH_ADDR_MASK));
}


