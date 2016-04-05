/*! \file
 *
 * This file contains the declarations for the instruction decoder of the
 * branching processor.  The instruction decoder extracts bits from the current
 * instruction and turns them into separate fields that go to various functional
 * units in the processor.
 */


#ifndef BRANCHING_DECODE_H
#define BRANCHING_DECODE_H


#include "bus.h"


/*!
 * The branching instruction decoder extracts bits from the instruction and
 * turns them into separate fields that go to various functional units in the
 * processor.
 */
typedef struct Decode {
    /* Inputs */
    pin input;
    /* Outputs */
    pin aluop;
    pin write;
    pin src1_addr;
    pin src2_addr;
    pin dst_addr;
    pin branch_addr;
} Decode;


/* Documentation appears in branching_decode.c. */
Decode * build_decode();
void free_decode(Decode *d);
void idecode(Decode *d);


#endif /* BRANCHING_DECODE_H */

