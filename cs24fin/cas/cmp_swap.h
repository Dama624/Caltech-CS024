#ifndef CMPANDSWAP_H
#define CMPANDSWAP_H

#include <stdint.h>


/* This function compares the value of *target to old, and if they are the same,
 * it stores new into *target and returns 1.  Otherwise, the function does not
 * modify *target, and returns 0.
 *
 * The compare-and-swap operation is performed atomically so that it will be
 * correct, even in the context of multiple processors.
 */
int compare_and_swap(uint32_t *target, uint32_t old, uint32_t new);


#endif /* CMPANDSWAP_H */
