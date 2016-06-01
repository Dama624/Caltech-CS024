/*============================================================================
 * A test program for exercising the virtual memory system by generating a
 * couple of matrices and then performing matrix multiplication into a third
 * matrix.
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "virtualmem.h"
#include "vmalloc.h"
#include "matrix.h"

#define DEFAULT_MAX_RESIDENT 64


static long seed = 0;
static unsigned int max_resident = DEFAULT_MAX_RESIDENT;
static int size;


/* Prints the test program's usage, and then exit the program. */
void usage(const char *prog) {
    printf("usage: %s [--seed num] [--max_resident num] size\n", prog);
    printf("\tRuns the test program, generating two square matrices with size\n");
    printf("\trows and columns, and then multiplying them into a third matrix.\n\n");
    printf("\t--seed | -s num optionally specifies the seed for the random\n");
    printf("\tnumber generator.  The system time is used otherwise.\n\n");
    printf("\t--max_resident | -m num specifies the maximum number of pages\n");
    printf("\tthat may be resident in the virtual memory system.\n");
    exit(1);
}


/* Parse the command-line arguments passed to the program. */
void parse_args(int argc, char **argv) {
    int c;

    while (1) {
        static struct option long_options[] = {
            /* These options set a flag.
            {"verbose", no_argument,       &verbose_flag, 1},
             */
            /* These options don’t set a flag.
             * We distinguish them by their indices. */
            {"seed",         required_argument, 0, 's'},
            {"max_resident", required_argument, 0, 'm'},
            {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "s:m:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 0:
            /* If this option set a flag, do nothing else now. */
            if (long_options[option_index].flag != 0)
                break;

            printf("option %s", long_options[option_index].name);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            break;

        case 's':
            seed = atol(optarg);
            printf("Setting seed to %ld\n", seed);
            break;

        case 'm':
            max_resident = atoi(optarg);
            printf("Max resident pages = %d\n", max_resident);
            break;

        case '?':
            /* getopt_long already printed an error message. */
            usage(argv[0]);
            /* usage() will exit the program. */
            break;

        default:
            abort();
        }
    }

    if (optind + 1 != argc)
        usage(argv[0]);

    size = atoi(argv[optind]);
}


int main(int argc, char **argv) {
    matrix_t *m1, *m2, *result;     /* Allocated from virtual memory pool */
    matrix_t *m1v, *m2v, *resultv;  /* Allocated with malloc(), to verify */

    /* Parse arguments */
    parse_args(argc, argv);

    /* Configure the test. */

    if (seed == 0)
       seed = time(NULL);

    printf("Options:\n");
    printf(" * Random seed = %ld\n", seed);
    printf(" * Max resident pages = %u\n", max_resident);
    printf(" * Using %d x %d matrices\n", size, size);
    printf("\n");

    srand(seed);

    /* Initialize the virtual memory system. */
    vmem_init(max_resident);
    vmem_alloc_init();

    /* Perform the test. */

    printf("Generating two matrices\n\n");

    /* Allocate matrices from the two memory sources.  Generate values into
     * the malloc()'d matrix since we can trust it.  Then copy the values into
     * the vmalloc()'d matrix.
     */

    m1v = malloc_matrix(size, size);
    m1 = vmalloc_matrix(size, size);
    generate_matrix_values(m1v);
    copy_matrix(m1v, m1);

    m2v = malloc_matrix(size, size);
    m2 = vmalloc_matrix(size, size);
    generate_matrix_values(m2v);
    copy_matrix(m2v, m2);

    resultv = malloc_matrix(size, size);
    result = vmalloc_matrix(size, size);

    printf("Multiplying the matrices together\n");
    printf(" * Printing one dot per row in result matrix.\n\n");

    /* Multiply the vmalloc()'d matrices and the malloc()'d matrices
     * separately, so that we can compare the results.
     */
    multiply_matrices(m1, m2, result);
    multiply_matrices(m1v, m2v, resultv);

    printf("Verifying source and result matrix contents\n");
    if (compare_matrices(m1, m1v))
        printf(" * Matrix m1 is correct\n");
    else
        printf(" * ERROR:  Matrix m1 doesn't contain correct values!\n");

    if (compare_matrices(m2, m2v))
        printf(" * Matrix m2 is correct\n");
    else
        printf(" * ERROR:  Matrix m2 doesn't contain correct values!\n");

    if (compare_matrices(result, resultv))
        printf(" * Result matrix is correct\n");
    else
        printf(" * ERROR:  Result matrix doesn't contain correct values!\n");

    printf("\nDone!\n\n");

    printf("Total page loads:  %u\n", get_num_loads());
    return 0;
}

