/*! \file
 *
 * This file contains all constants associated with instructions in the simple
 * processor.
 */


/**** REGISTER-FILE CONSTANTS ****/


/*! Specifies the number of address bits for the register file. */
#define REGISTER_BITS 3

/*!
 * Using the number of register-bits, this expression is the total number of
 * registers in the register file.
 */
#define NUM_REGISTERS (1 << REGISTER_BITS)


/**** INSTRUCTION-ENCODING CONSTANTS ****/

/*
 * These are masks for selecting exactly the bits associated with different
 * parts of the instruction.
 */

#define ALUOP_MASK 0x0f
#define WRITE_MASK 0x01
#define SRC1_MASK (NUM_REGISTERS - 1)
#define SRC2_MASK (NUM_REGISTERS - 1)
#define DST_MASK  (NUM_REGISTERS - 1)

#define DST_POS   0
#define SRC2_POS  ( DST_POS + REGISTER_BITS)
#define SRC1_POS  (SRC2_POS + REGISTER_BITS)
#define WRITE_POS (SRC1_POS + REGISTER_BITS)
#define ALUOP_POS (WRITE_POS + 1)


/**** ALU OPCODES ****/


#define ALUOP_ADD  0x00  /*!< The opcode for simple addition. */
#define ALUOP_INV  0x01  /*!< The opcode for bitwise invert. */
#define ALUOP_SUB  0x02  /*!< The opcode for simple subtraction. */
#define ALUOP_XOR  0x03  /*!< The opcode for bitwise exclusive-or. */
#define ALUOP_OR   0x04  /*!< The opcode for bitwise or. */
#define ALUOP_INCR 0x05  /*!< The opcode for incrementing a register. */
#define ALUOP_AND  0x08  /*!< The opcode for bitwise and. */
#define ALUOP_SRA  0x0B  /*!< The opcode for arithmetic shift-right. */
#define ALUOP_SRL  0x0C  /*!< The opcode for logical shift-right. */

/*!
 * The opcode for arithmetic shift-left (same behavior as logical shift-left).
 */
#define ALUOP_SLA  0x0D

/*!
 * The opcode for logical shift-left (same behavior as arithmetic shift-left).
 */
#define ALUOP_SLL  0x0E

#define ALUOP_DONE 0x0F  /*!< The opcode for signalling the end of a program. */


/**** WRITE FLAG ****/


/*!
 * The value sent to the register-file to store the output of the ALU to a
 * register.
 */
#define WRITE   0x01

/*!
 * The value sent to the register-file to tell it not to store the ALU output.
 */
#define NOWRITE 0x00


/**** BRANCHING PROCESSOR CONSTANTS ****/


/*
 * These constants are for the branching processor, and are not necessary for
 * the simple processor.
 */

/*! The opcode for the "branch if not zero" instruction. */
#define ALUOP_BNZ  0x0A

/*!
 * The value that the branch-unit sends to the branching program-counter, when
 * the branch-unit determines that it's time to jump to a different address.
 */
#define BRANCH   0x01

/*!
 * The value that the branch-unit sends to the branching program-counter, when
 * the program counter should simply go on to the next instruction as usual.
 */
#define NOBRANCH 0x00

#define BRANCH_ADDR_POS 0
#define BRANCH_ADDR_MASK ((1 << (2 * REGISTER_BITS)) - 1)


