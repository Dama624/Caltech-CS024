#ifndef FFUNC_H
#define FFUNC_H

#include <stdio.h>


/* This data type simply holds a count of how many floats are in the
 * array, and also a pointer to the array of floating-point values
 * themselves.
 */
typedef struct FloatArray {
    int count;        /* The number of floats in the array. */
    float *values;    /* The array of floats itself. */
} FloatArray;


void load_floats(FILE *input, FloatArray *floats);

void sort_incmag(FloatArray *floats);
void sort_decmag(FloatArray *floats);

#endif /* FFUNC_H */

