#include "accum.h"
#include "cmp_swap.h"


/* Adds "value" to *accum, returning the new total that is held in *accum as
 * a result of this particular addition (i.e. irrespective of other concurrent
 * additions that may be occurring).
 *
 * This function should be thread-safe, allowing calls from multiple threads
 * without generating spurious results.
 */
uint32_t add_to_accum(uint32_t *accum, uint32_t value) {
	unsigned int result = 0;
	unsigned int old;
	while (!result)
	{
		old = *accum;
		result = compare_and_swap(accum, old, old + value);
	}
	return *accum;
}

