/*! \file
 *
 * This file contains definitions for the instruction store, which holds the
 * sequence of instructions that the processor executes.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction_store.h"


/*
 * Instruction Store
 *
 *  The Instruction Store is a memory that
 *   holds the instructions that describe
 *   the computation we wish to preform.
 *
 *  The Instruction Store can be loaded
 *    and then will return the value associated
 *    with each address.
 */

static int VERBOSE = 1;


InstructionStore *build_instruction_store() {
    InstructionStore *is = malloc(sizeof(InstructionStore));
    if (!is) {
        fprintf(stderr, "Out of memory building an instruction store!\n");
        exit(11);
    }
    memset(is, 0, sizeof(InstructionStore));
    return is;
}

void free_instruction_store(InstructionStore *is) {
    free(is);
}


/* Fetch an instruction from the memory */
void ifetch(InstructionStore *is) {
    pin_set(is->output, is->imemory[pin_read(is->address_input)]);
}


/* Read the instruction data from an open file descriptor.  Each
 * instruction is stored in ASCII as a hexadecimal number.
 * Closes the file descriptor when finished.
 */
void load_instruction_store_from_fd(FILE *fd, InstructionStore *is) {
    char buf[500];
    int slot = 0;
    int value;
    int statusp;

    if (VERBOSE > 0)
        printf("Loading instruction store.\n");

    while (slot < INSTRUCTION_STORE_DEPTH) {
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
            is->imemory[slot] = value;
            if (VERBOSE > 0)
                printf("Storing istore[%d] = %lX\n", slot, is->imemory[slot]);
            slot++;
        }
        /*
        else if (feof(fd)) {
            if (VERBOSE > 0)
                printf("Read complete.\n");
            break;
        }
        */
        else {
            fprintf(stderr, "Unexpected data in input file, skipping...\n");
        }
    }

    fclose(fd);
}


