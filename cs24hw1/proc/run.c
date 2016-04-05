/*! \file
 * This file contains the main entry point and wrapper for running the processor
 * simulator.  It contains a main() method that can be used for executing the
 * processor with a set of registers and instructions, and outputting the
 * results to a data file.
 */


#include <stdio.h>
#include <stdlib.h>

#include "instruction_store.h"
#include "register_file.h"


#ifdef BRANCHING
#include "branching_processor.h"
#else
#include "simple_processor.h"
#endif



/*! Run the processor against an initial state and set of instructions. */
int main (int argc,  char **argv) {
    FILE *ifd;     /* File for loading the instructions from. */
    FILE *rifd;    /* File for loading the initial register-file from. */
    FILE *rofd;    /* File for storing the final register-file to. */
    Processor *proc;  /* The processor state to run with. */

    if (argc < 4) {
        fprintf(stderr, "Usage: run instruction-file "
                        "initial-register-file-contents "
                        "final-register-file-contents\n");
        exit(1);
    }

    proc = build_processor();

    ifd = fopen(argv[1], "r");
    if (!ifd) {
        fprintf(stderr, "Opening %s: ", argv[1]);
        perror("fopen");
        exit(2);
    }
    load_instruction_store_from_fd(ifd, proc->is);

    rifd = fopen(argv[2], "r");
    if (!rifd) {
        fprintf(stderr, "Opening %s: ", argv[2]);
        perror("fopen");
        exit(2);
    }
    load_register_file_from_fd(rifd, proc->rf);

    rofd = fopen(argv[3], "w");
    if(!rofd) {
        fprintf(stderr, "Opening %s: ", argv[3]);
        perror("fopen");
        exit(2);
    }

    run(proc);

    write_register_file_to_fd(rofd, proc->rf);
    fclose(rofd);

    free_processor(proc);

    return 0;
}

