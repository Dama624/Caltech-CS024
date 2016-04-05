/*! \file
 */
 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "register_file.h"


/*
 * Register File
 *
 *  The Register File is a memory that
 *   holds intermediate values for the computation.
 *
 *  The Register File can be read and written.
 *    This is a 2 read port, 1 write port register file.
 */

static int VERBOSE = 1;


/*!
 * 
 */
RegisterFile* build_register_file() {
    RegisterFile *rf = malloc(sizeof(RegisterFile));
    if(!rf) {
        fprintf(stderr, "Out of memory building a register file!\n");
        exit(11);
    }
    memset(rf, 0, sizeof(RegisterFile));
    return rf;
}

void free_register_file(RegisterFile* rf) { free(rf); }

/* Set output pins to the values stored at src1_addr and src2_addr.
 */
void rf_read(RegisterFile* rf) {
   pin_set(rf->src1, rf->rfmem[pin_read(rf->src1_addr)]);
   pin_set(rf->src2, rf->rfmem[pin_read(rf->src2_addr)]);
}

/* If the write pin is set to WRITE, store the value on the "dst" pin
 * at the address on the dst_addr pin.
 */
void rf_write(RegisterFile* rf) {
   if(pin_read(rf->writep) == WRITE)
      rf->rfmem[pin_read(rf->dst_addr)] = pin_read(rf->dst);
}


/*!
 * Read the register file data from an open file descriptor.  Each instruction
 * is stored in ASCII as a hexadecimal number.  Closes the file descriptor when
 * finished.
 */
void load_register_file_from_fd(FILE *fd, RegisterFile *rf) {
    char buf[500];
    int slot = 0;
    uint32_t value;
    int statusp;

    if (VERBOSE > 0)
        printf("Loading register file.\n");
    
    
    while (slot < NUM_REGISTERS) {
        if (!fgets(buf, sizeof(buf), fd)) {
            if (VERBOSE > 0)
                printf("Read complete.\n");
            break;
        }
        
        /* Initial space in format specifier allows for whitespace before values
         * in the input line.
         */
        statusp = sscanf(buf, " %x", &value);

        if (statusp == 1) {
            rf->rfmem[slot] = value;
            if (VERBOSE > 0)
                printf("Storing rfmem[%d] = %X\n", slot, value);
            slot++;
        }
        /*
        else if (feof(fd)) {
            if (VERBOSE > 0)
                printf("read complete\n");
            break;
        }
        */
        else {
            fprintf(stderr, "Unexpected data in input file, skipping...\n");
        }
    }

    fclose(fd);
}


/*! Write the contents of a register file to an open file descriptor. */
void rfprint(FILE* fd, RegisterFile* rf) {
    int i;

    for (i = 0; i < NUM_REGISTERS; i++)
        fprintf(fd, "%d:%0X ", i, rf->rfmem[i]);

    fprintf(fd,"\n");
}

/* Write the contents of a register file to an open file descriptor.
 */
void write_register_file_to_fd(FILE *fd, RegisterFile *rf) {
    int i;
    for (i=0; i < NUM_REGISTERS; i++) {
        fprintf(fd, "%0X\n", rf->rfmem[i]);
    }
}
