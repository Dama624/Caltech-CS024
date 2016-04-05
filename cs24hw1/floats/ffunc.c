#include "ffunc.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>


/* Load a sequence of floating-point values from the specified
 * input file.  The format must be as follows:
 *     Line 1:       Number of values N
 *     Line 2..N+1:  The floating-point values themselves
 *
 * The values themselves are stored into the specified FloatArray
 * struct, which functions as an in/out parameter.
 */
void load_floats(FILE *input, FloatArray *floats) {
    int count, i;
    float value;

    assert(input != NULL);
    assert(floats != NULL);

    if (fscanf(input, "%d", &count) != 1) {
        printf("Error:  couldn't read count from input list\n");
        exit(1);
    }

    if (count <= 0) {
        printf("ERROR:  count must be positive; got %d\n", count);
        exit(1);
    }

    float *values = malloc(count * sizeof(float));
    if (values == NULL) {
        printf("ERROR:  couldn't allocate %u bytes!\n",
               (unsigned int) count * sizeof(float));
        exit(1);
    }

    /* Zero out the newly allocated memory. */
    memset(values, 0, count * sizeof(float));

    /* Load each floating-point value into its corresponding location
     * in the array.
     */
    for (i = 0; i < count; i++) {
        if (fscanf(input, "%f", &value) != 1) {
            printf("ERROR:  couldn't read a value from input list\n");
            exit(1);
        }

        values[i] = value;
    }

    /* Finally, store the loaded values into the passed-in struct. */
    floats->count = count;
    floats->values = values;
}


/* This comparison function can be used with the qsort() utility function
 * to order an array of floating-point values by increasing magnitude.  The
 * comparison function must return a negative value if arg 1 < arg 2,
 * zero if arg 1 == arg 2, and a positive value if arg 1 > arg 2.
 */
int cmp_inc_fmag(const void *pf1, const void *pf2) {
    /* To get at the floats, cast the void-pointers to float-pointers,
     * then dereference to get to the float values themselves.
     */
    float f1 = *((float *) pf1);
    float f2 = *((float *) pf2);

    f1 = fabsf(f1);  /* Drop the signs of both f1 and f2. */
    f2 = fabsf(f2);

    if (f1 < f2)
        return -1;
    else if (f1 > f2)
        return 1;
    else
        return 0;
}

/* This comparison function can be used with the qsort() utility function
 * to order an array of floating-point values by increasing magnitude.  The
 * comparison function must return a negative value if arg 1 < arg 2,
 * zero if arg 1 == arg 2, and a positive value if arg 1 > arg 2.
 */
int cmp_dec_fmag(const void *pf1, const void *pf2) {
    /* To reverse the order of the comparison, just reverse the inputs. */
    return cmp_inc_fmag(pf2, pf1);
}


/* This helper function sorts the input float-array by increasing magnitude,
 * using the qsort() utility function and the cmp_inc_fmag comparison function.
 */
void sort_incmag(FloatArray *floats) {
    assert(floats != NULL);
    qsort(floats->values, floats->count, sizeof(float), cmp_inc_fmag);
}


/* This helper function sorts the input float-array by decreasing magnitude,
 * using the qsort() utility function and the cmp_dec_fmag comparison function.
 */
void sort_decmag(FloatArray *floats) {
    assert(floats != NULL);
    qsort(floats->values, floats->count, sizeof(float), cmp_dec_fmag);
}

