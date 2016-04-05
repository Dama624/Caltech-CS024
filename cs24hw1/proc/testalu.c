/*! \file
 *
 * This file contains some basic test code for exercising the arithmetic/logic
 * unit (ALU).  Various operations are performed, and then the results are
 * output to the console for double-checking.
 */

#include <stdio.h>

#include "bus.h"
#include "instruction.h"
#include "alu.h"


#define OUTPUT_ALU_3_PINS(IN1, OP, IN2, OUT) \
    printf("\t0x%X (%d) %s 0x%X (%d) = 0x%X (%d)\n", \
         (IN1), (int32_t) (IN1), (OP), (IN2), (int32_t) (IN2), (OUT), (int32_t) (OUT))

#define OUTPUT_ALU_2_PINS(IN1, OP, OUT) \
    printf("\t0x%X (%d) %s = 0x%X (%d)\n", \
         (IN1), (int32_t) (IN1), (OP), (OUT), (int32_t) (OUT))


/*! Routine for exercising the ALU with a number of simple inputs. */
void testops(ALU* alu, pin in1, pin in2, pin op, pin out) {
    pin_set(op, ALUOP_INV);
    alu_eval(alu);
    printf("\t~0x%X = 0x%X\n", pin_read(in1), pin_read(out));

    pin_set(op, ALUOP_ADD);
    alu_eval(alu);
    OUTPUT_ALU_3_PINS(pin_read(in1), "+", pin_read(in2), pin_read(out));

    pin_set(op, ALUOP_SUB);
    alu_eval(alu);
    OUTPUT_ALU_3_PINS(pin_read(in1), "-", pin_read(in2), pin_read(out));

    pin_set(op, ALUOP_INCR);
    alu_eval(alu);
    OUTPUT_ALU_2_PINS(pin_read(in1), "+ 1", pin_read(out));

    pin_set(op, ALUOP_AND);
    alu_eval(alu);
    OUTPUT_ALU_3_PINS(pin_read(in1), "&", pin_read(in2), pin_read(out));

    pin_set(op, ALUOP_OR);
    alu_eval(alu);
    OUTPUT_ALU_3_PINS(pin_read(in1), "|", pin_read(in2), pin_read(out));

    pin_set(op, ALUOP_XOR);
    alu_eval(alu);
    OUTPUT_ALU_3_PINS(pin_read(in1), "^", pin_read(in2), pin_read(out));

    pin_set(op, ALUOP_SLL);
    alu_eval(alu);
    OUTPUT_ALU_2_PINS(pin_read(in1), "<< 1 (logical)", pin_read(out));

    pin_set(op, ALUOP_SRL);
    alu_eval(alu);
    OUTPUT_ALU_2_PINS(pin_read(in1), ">> 1 (logical)", pin_read(out));

    pin_set(op, ALUOP_SLA);
    alu_eval(alu);
    OUTPUT_ALU_2_PINS(pin_read(in1), "<< 1 (arithmetic)", pin_read(out));

    pin_set(op, ALUOP_SRA);
    alu_eval(alu);
    OUTPUT_ALU_2_PINS(pin_read(in1), ">> 1 (arithmetic)", pin_read(out));
}


/*! Entry-point for the ALU testing program. */
int main (int argc, char **argv) {
    pin op, in1, in2, out;
    ALU *alu;

    /* Build up the ALU and its buses. */
    alu = build_alu();
    connect(&op, &alu->op);
    connect(&in1, &alu->in1);
    connect(&in2, &alu->in2);
    connect(&out, &alu->out);

    printf("This program will not report success or failure.\n");
    printf("You must inspect the results to see if they are correct.\n\n");

    printf("Testing ALU with in1 = 24, in2 = 20.\n\n");
    pin_set(in1, 24);
    pin_set(in2, 20);
    testops(alu, in1, in2, op, out);

    printf("\nTesting ALU with in1 = 20, in2 = 24.\n\n");
    pin_set(in1, 20);
    pin_set(in2, 24);
    testops(alu, in1, in2, op, out);

    printf("\nTesting ALU with in1 = -1, in2 = -3.\n\n");
    pin_set(in1, -1);
    pin_set(in2, -3);
    testops(alu, in1, in2, op, out);

    printf("\nTesting ALU with in1 = -2, in2 = 4.\n\n");
    pin_set(in1, -2);
    pin_set(in2, 4);
    testops(alu, in1, in2, op, out);

    free_alu(alu);
    
    return 0;
}

