#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "ffunc.h"


/* This function takes an array of single-precision floating point values,
 * and computes a sum in the order of the inputs.  Very simple.
 */
float fsum(FloatArray *floats) {
    float sum = 0;
    int i;

    for (i = 0; i < floats->count; i++)
        sum += floats->values[i];

    return sum;
}


/* DONE:  IMPLEMENT my_fsum() HERE, AND DESCRIBE YOUR APPROACH. */
float my_fsum(FloatArray *floats) {
    float final_sum = 0.0; // will contain final value to return
    float temp_input; // input modified w/ compensation for lost precision
    float temp_sum; // sum of final_sum and temp_input
    float compensation = 0.0; // compensation for lost precision
    unsigned int i; // counter for for loop
    for (i = 0; i < floats->count; i++)
    {
        // first run: no precision/digits lost
        temp_input = floats->values[i] - compensation;
        // if temp_input and final_sum have big magnitude difference,
        // precision (and consequently digits) are lost
        temp_sum = final_sum + temp_input;
        // (temp_sum - final_sum) removes greater sig figs
        // (- temp_input) recovers lost digits by removing higher order sig
        // figs
        compensation = (temp_sum - final_sum) - temp_input;
        final_sum = temp_sum;
    }
    return final_sum;
}


int main() {
    FloatArray floats;
    float sum1, sum2, sum3, my_sum;

    load_floats(stdin, &floats);
    printf("Loaded %d floats from stdin.\n", floats.count);

    /* Compute a sum, in the order of input. */
    sum1 = fsum(&floats);

    /* Use my_fsum() to compute a sum of the values.  Ideally, your
     * summation function won't be affected by the order of the input floats.
     */
    my_sum = my_fsum(&floats);

    /* Compute a sum, in order of increasing magnitude. */
    sort_incmag(&floats);
    sum2 = fsum(&floats);

    /* Compute a sum, in order of decreasing magnitude. */
    sort_decmag(&floats);
    sum3 = fsum(&floats);

    /* %e prints the floating-point value in full precision,
     * using scientific notation.
     */
    printf("Sum computed in order of input:  %e\n", sum1);
    printf("Sum computed in order of increasing magnitude:  %e\n", sum2);
    printf("Sum computed in order of decreasing magnitude:  %e\n", sum3);

    // DONE:  UNCOMMENT WHEN READY!
    printf("My sum:  %e\n", my_sum);
    

    return 0;
}

