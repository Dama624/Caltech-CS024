#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rl_decode.h"


void usage(const char *progname) {
    assert(progname != NULL);

    printf("usage:  %s infile outfile\n", progname);
    printf("\tThe program takes a run-length-encoded input file and\n");
    printf("\tproduces a decoded version of the file, saving\n");
    printf("\tthe result to outfile.\n");
}


int main(int argc, char **argv) {
    FILE *input, *output;
    int input_size, output_size;
    char *input_buffer, *output_buffer;

    /* If we didn't get enough arguments, complain. */
    if (argc != 3) {
        usage(argv[0]);
        return 1;
    }

    input = fopen(argv[1], "rb");
    if (input == NULL) {
        printf("Couldn't open input file \"%s\"!\n", argv[1]);
        return 2;
    }

    output = fopen(argv[2], "wb");
    if (output == NULL) {
        printf("Couldn't open output file \"%s\"!\n", argv[2]);
        return 3;
    }

    printf("Decoding file \"%s\" into file \"%s\".\n", argv[1], argv[2]);

    /* Load the input file into a buffer in memory. */
    fseek(input, 0, SEEK_END);
    input_size = ftell(input);
    fseek(input, 0, SEEK_SET);
    input_buffer = malloc(input_size + 1);

    fread(input_buffer, sizeof(unsigned char), input_size, input);

    /* Decode the input file, and write the resutls to the output file. */
    output_buffer = rl_decode(input_buffer, input_size, &output_size);

    fwrite(output_buffer, sizeof(unsigned char), output_size, output);

    printf("All done!\n");

    fclose(output);
    fclose(input);

    free(input_buffer);
    free(output_buffer);

    return 0;
}

