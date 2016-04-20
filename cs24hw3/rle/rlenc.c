#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void usage(const char *progname) {
    assert(progname != NULL);

    printf("usage:  %s infile outfile\n", progname);
    printf("\tThe program takes the input file and produces a\n");
    printf("\trun-length-encoded version of the file, saving\n");
    printf("\tthe result to outfile.\n");
}


int main(int argc, char **argv) {
    FILE *input, *output;
    int lastch, nextch, count;

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

    printf("Encoding file \"%s\" into file \"%s\".\n", argv[1], argv[2]);

    /* Encode the input file, and write the resutls to the output file. */

    lastch = fgetc(input);
    if (lastch != EOF) {
        count = 1;

        while (1) {
            nextch = fgetc(input);
            if (nextch == EOF)
                break;

            assert(count > 0 && count < 256);

	    if ((nextch != lastch && lastch != -1) || count == 255) {
                /* Write out the count and then the character. */
                fputc(count, output);
                fputc(lastch, output);

                lastch = nextch;
                count = 1;
            }
            else {
                /* Saw the same character again. */
                count++;
            }

            assert(count > 0 && count < 256);
        }

        if (count > 0) {
            /* Write the last values out. */
            fputc(count, output);
            fputc(lastch, output);
        }
    }

    fclose(output);
    fclose(input);

    printf("All done!\n");

    return 0;
}

