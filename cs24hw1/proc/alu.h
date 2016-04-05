/*! \file
 *
 * This file contains declarations for an Arithmetic/Logic Unit of an
 * emulated processor.
 */


#ifndef ALU_H
#define ALU_H


#include "bus.h"


/*!
 * This structure represents the data inputs, outputs, and operation code
 * ("opcode") that feed the Arithmetic/Logic Unit (ALU).  The ALU implementation
 * (in alu.c) takes the inputs and computes a result, based on the specific
 * operation fed via the control inputs.
 *
 * This particular ALU handles 32 bit inputs and generates a 32 bit output.
 */
typedef struct ALU {
   /* Inputs */
   pin in1;     /*!< The first data input to the ALU. */
   pin in2;     /*!< The second data input to the ALU. */
   pin op;      /*!< The operation code, a.k.a. "opcode". */

   /* Output */
   pin out;     /*!< The data output from the ALU. */
} ALU;


/* Documentation appears in alu.c. */
ALU * build_alu();
void free_alu(ALU *alu);
void alu_eval(ALU *alu);


#endif /* ALU_H */

