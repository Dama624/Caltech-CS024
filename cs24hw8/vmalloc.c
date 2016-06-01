/*============================================================================
 * Implementation of the simple allocator that sits on top of the virtual
 * memory system.
 */

#include <stdio.h>

#include "virtualmem.h"
#include "vmalloc.h"


/* Just like the "unacceptable allocator" in HW3, we start our pointer at
 * the start of virtual memory, and just move it forward by the requested
 * allocation size, as long as the request will fit in the virtual memory
 * area.  When we run out of space, we start returning NULL.
 *
 * Oh, and we never deallocate...
 */
static void *nextptr;


/* Initialize the simple memory allocator. */
void vmem_alloc_init() {
    nextptr = get_vmem_start();
}


/* Request an allocation of the specified size.  This will return NULL if
 * no more memory is available.
 */
void * vmem_alloc(unsigned int size) {
    void *p;

    if (nextptr + size > get_vmem_end()) {
        fprintf(stderr, "vmem_alloc(%u): ran out of heap space\n", size);
        return NULL;
    }

    p = nextptr;
    nextptr += size;

    return p;
}

