/*! \file
 *
 * This file contains declarations for the register file, which holds the values
 * of all registers, and allows specific registers to be read from and written
 * to.
 */


#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H


#include "bus.h"
#include "instruction.h"


/*!
 * This struct declares the register file, a small and fast memory that holds
 * intermediate values for a computation.  The register file can be read and
 * written.  There are two read ports and one write port.
 */
typedef struct RegisterFile {
    /* Inputs */

    /*!
     * This pin tells the register file whether it should actually write the
     * value on the dst pin to a register in the file.
     */
    pin writep;

    /*!
     * This input pin specifies the address of the first register to output.
     * The register's output is made available on the src1 output pin.
     */
    pin src1_addr;


    /*!
     * This input pin specifies the address of the second register to output.
     * The register's output is made available on the src2 output pin.
     */
    pin src2_addr;


    /*!
     * This input pin specifies the address of the register to write a value to.
     * The value to store to the register is available on the dst input pin.
     */
    pin dst_addr;

    /*!
     * This input pin specifies the value to write to the destination register.
     * For this basic processor, the value comes directly from the ALU.
     */
    pin dst; /* Result from ALU */


    /* Outputs */

    /*! This pin is where the first source register's value is output to. */
    pin src1;

    /*! This pin is where the second source register's value is output to. */
    pin src2;

    /*! These are the actual register values, stored in an array. */
    uint32_t rfmem[NUM_REGISTERS];

} RegisterFile;


/* Documentation appears in register_file.c. */

RegisterFile * build_register_file();
void free_register_file(RegisterFile *rf);

void rf_read(RegisterFile *rf);
void rf_write(RegisterFile *rf);
void rfprint(FILE *fd, RegisterFile *rf);

void load_register_file_from_fd(FILE *fd, RegisterFile *rf);
void write_register_file_to_fd(FILE *fd, RegisterFile *rf);


#endif /* REGISTER_FILE_H */


