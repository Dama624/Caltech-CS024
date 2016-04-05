/*! \file
 *
 * This file contains definitions for an Arithmetic/Logic Unit of an
 * emulated processor.
 */


#include <stdio.h>
#include <stdlib.h>   /* malloc(), free() */
#include <string.h>   /* memset() */

#include "alu.h"
#include "instruction.h"


/*!
 * This function dynamically allocates and initializes the state for a new ALU
 * instance.  If allocation fails, the program is terminated.
 */
ALU * build_alu() {
    /* Try to allocate the ALU struct.  If this fails, report error then exit. */
    ALU *alu = malloc(sizeof(ALU));
    if (!alu) {
        fprintf(stderr, "Out of memory building an ALU!\n");
        exit(11);
    }

    /* Initialize all values in the ALU struct to 0. */
    memset(alu, 0, sizeof(ALU));
    return alu;
}


/*! This function frees the dynamically allocated ALU instance. */
void free_alu(ALU *alu) {
    free(alu);
}


/*!
 * This function implements the logic of the ALU.  It reads the inputs and
 * opcode, then sets the output accordingly.  Note that if the ALU does not
 * recognize the opcode, it should simply produce a zero result.
 */
void alu_eval(ALU *alu) {
    uint32_t A, B, aluop;
    uint32_t result;

    A = pin_read(alu->in1);
    B = pin_read(alu->in2);
    aluop = pin_read(alu->op);

    result = 0;

    /*======================================*/
    /* DONE:  Implement the ALU logic here. */
    /*======================================*/
    // Account for each ALU operation
    switch (aluop)
    {
        case ALUOP_ADD:
            // Simple addition
            result = A + B;
            break;
        case ALUOP_INV:
            // Bitwise inversion. Only applies to A!
            result = ~A;
            break;
        case ALUOP_SUB:
            // Simple subtraction
            result = A - B;
            break;
        case ALUOP_XOR:
            // Bitwise exclusive or
            result = A ^ B;
            break;
        case ALUOP_OR:
            // Bitwise or
            result = A | B;
            break;
        case ALUOP_INCR:
            // Incrementing a register
            result = A + 1;
            break;
        case ALUOP_AND:
            // Bitwise and
            result = A & B;
            break;
        case ALUOP_SRA:
            // Arithmetic shift right
            // According to the Problem Set appendix, the leading bit must
            // be the same as the source's (despite uint32_t typed source)
            result = A >> 1;
            // Mask with 0x80000000 to check if leading bit of source is 1
            if ((A & 0x80000000) == 0)
            {
                // Leading bit of source is 0
                // Leading bit of result must be 0 too
                result &= 0x7FFFFFFF;
            }
            else
            {
                // Leading bit of source is 1
                // Leading bit of result must be 1 too
                result |= 0x80000000;
            }
            break;
        case ALUOP_SRL:
            // Logical shift right
            // Our source types are uint32_t (unsigned) so C defaults >> to
            // unsigned; we set the leading bit to 0 just in case anyway
            result = A >> 1;
            result &= 0x7FFFFFFF;
            break;
        case ALUOP_SLA:
            // Arithmetic shift left
            // According to the Problem Set appendix, this is just a <<
            // (i.e. no distinction made between Arithmetic and Logical)
            result = A << 1;
            break;
        case ALUOP_SLL:
            // Logical shift left
            // See above
            result = A << 1;
            break;
    }

    pin_set(alu->out, result);
}

