/*! \file
 *
 * This file specifies all the data structures and functions for simulating a
 * branching processor.
 */


#ifndef BRANCHING_PROCESSOR_H
#define BRANCHING_PROCESSOR_H


#include "bus.h"
#include "alu.h"
#include "register_file.h"
#include "instruction_store.h"


#include "branching_program_counter.h"
#include "branching_decode.h"
#include "branch_unit.h"


/*!
 * This struct encapsulates all of the components of the branching processor.
 */
typedef struct Processor {
    ProgramCounter *pc;    /*!< Keeps track of the current instruction. */
    InstructionStore *is;  /*!< Holds all of the instructions of the program. */
    Decode *decode;        /*!< Breaks instructions down into components. */
    RegisterFile *rf;      /*!< Holds the register state. */
    ALU *alu;              /*!< Performs computations in the processor. */
    BranchUnit *bru;       /*!< Determines when the processor should branch. */

    /*! Bus that stores and exposes the program counter. */
    bus pc_bus;

    /*! Bus that stores and exposes the current instruction. */
    bus instr;

    /*! Buses that expose various components of the decoded instruction. */
    bus aluop, writep, src1, src2, dst;
    
    /*!
     * These two buses are where the register file writes out its two operands.
     */
    bus rf1, rf2;
    
    /*! Bus where the ALU writes its output to. */
    bus aluout;
    
    /*! Buses related to branching details. */
    bus branch_addr, branch;
} Processor;


/* Documentation appears in branching_processor.c. */
Processor * build_processor();
void free_processor(Processor *proc);
void run(Processor *proc);


#endif /* BRANCHING_PROCESSOR_H */

