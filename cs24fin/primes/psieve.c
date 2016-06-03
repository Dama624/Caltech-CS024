#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bitvector.h"


#define DEFAULT_MAX_VALUE 1000000000


unsigned int bit_index_of(unsigned int n);
void find_primes(unsigned int max_value);


/*!
 * Prints out the program's usage.  Takes the program's name as an argument.
 */
void usage(const char *program) {
    fprintf(stderr, "usage:  %s [max_value]\n", program);
    fprintf(stderr, "\tFinds and prints all prime numbers up to max_value.\n");
    fprintf(stderr, "\tIf max_value is unspecified, %u is used.\n",
            DEFAULT_MAX_VALUE);
}


/*!
 * The program takes an optional argument specifying how high to go when
 * searching for primes, and then uses the Sieve of Eratosthenes approach
 * to generate these primes.
 */
int main(int argc, char **argv) {
    char *pch;
    unsigned int max_value;

    /* Figure out the maximum value to search to. */
    if (argc == 2) {
        max_value = (unsigned int) strtoul(argv[1], &pch, 10);
        if (*pch != 0 || max_value == 0) {
            usage(argv[0]);
            return 1;
        }
    }
    else if (argc == 1) {
        max_value = DEFAULT_MAX_VALUE;
    }
    else {
        usage(argv[0]);
        return 1;
    }
    fprintf(stderr, "Using maximum value of %u\n", max_value);

    find_primes(max_value);

    return 0;
}


/*!
 * This helper function computes the bit-index for the specified number.
 * Note that no even numbers are represented in the bit-vector, since we
 * already know they are not prime.  Thus, n must be > 2, and must be odd.
 * The index of a value n is simply (n - 3) / 2.  (This is fast to compute.)
 */
unsigned int bit_index_of(unsigned int n) {
    assert((n & 1) == 1);    /* Assume numbers must always be odd! */
    assert(n > 2);           /* Numbers must be > 2 as well. */

    return (n - 3) / 2;
}


/*!
 * This function actually implements the prime number sieve.  A bit-vector is
 * used to represent individual numbers, so each 32-bit unsigned integer can
 * represent 32 different numbers.  In addition, we don't represent even
 * numbers in our bit-vector since we already know that any even number > 2
 * is non-prime.  Thus, we can find a very large number of primes using a
 * relatively small amount of memory.
 */
void find_primes(unsigned int max_value) {
    bitvector v;
    unsigned int p, primes_found;

    /* Create a large enough bit-vector, then set all bits to 1 initially.
     * Since we only track odd numbers, we only need (approximately)
     * max_value / 2 bits in the vector.
     */
    if (!init_bitvector(&v, max_value / 2)) {
        fprintf(stderr, "Couldn't allocate memory for bit-vector!\n");
        exit(2);
    }
    set_all_bits(&v, 1);

    /* Start finding primes!  2 is a special case, so print it out right away,
     * then use a sieve to find each subsequent prime.
     */

    printf(" %d", 2);
    primes_found = 1;

    /* The first sqrt(max_value) numbers could be prime, and we need to
     * handle them separately from the numbers larger than sqrt(max_value).
     */
    for (p = 3;
         (unsigned long long int) p * p <= (unsigned long long int) max_value;
         p += 2) {

        /* If the bit for p is still set, it's prime.  Print it out, then
         * clear all bits that are for multiples of p, through to the end
         * of the bit-vector.
         */
        if (get_bitvalue(&v, bit_index_of(p))) {
            unsigned int multiple, prev, incr;

            printf(" %u", p);
            primes_found++;

            /* Starting with p * p, start clearing out the bits for multiples
             * of the prime number p.  Note that we don't store bits for even
             * numbers in the bit-vector, but some multiples of p are even,
             * e.g. 2p, 4p, 6p, etc.  So, we start at p * p (which is odd),
             * and increment by 2p each time so that we only look at the odd
             * multiples of p.  It gets rid of an if-statement inside the
             * loop, which definitely makes it faster!
             */
            prev = p;
            multiple = p * p;
            incr = 2 * p;
            while (multiple > prev && multiple < max_value) {
                set_bitvalue(&v, bit_index_of(multiple), 0);
                prev = multiple;
                multiple += incr;
            }
        }
    }

    /* Once we get to the values p > sqrt(max_value), if p is a non-prime
     * then its bit is already cleared because it will have some factor q < p
     * that has already been applied as a sieve.
     */
    for ( /* nothing */ ; p < max_value; p += 2) {
        if (get_bitvalue(&v, bit_index_of(p))) {
            printf(" %u", p);
            primes_found++;
        }
    }

    printf("\n");

    fprintf(stderr, "\nTotal primes found:  %u\n", primes_found);
    free_bitvector(&v);
}

