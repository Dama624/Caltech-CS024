#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "memory.h"


/* Set this to a nonzero value and rebuild to see debug output. */
#define DEBUG_MEMORY 0


/* Local functions used by the memory implementation. */

unsigned char memory_read_byte(membase_t *mb, addr_t address);
void memory_write_byte(membase_t *mb, addr_t address, unsigned char value);
void memory_print_stats(membase_t *mb);
void memory_reset_stats(membase_t *mb);
void memory_free(membase_t *mb);


/* Initializes the members of the memory_t struct to be a memory of the
 * specified number of bytes.  This requires a heap allocation, so the
 * allocated memory must be released when cleaning up the memory.
 */
void init_memory(memory_t *p_memory, int32_t mem_size) {
    bzero(p_memory, sizeof(memory_t));

    /* Allocate the memory, and clear its contents to 0. */
    p_memory->mem_size = mem_size;
    p_memory->mem = malloc(mem_size);
    bzero(p_memory->mem, mem_size);

    /* Set up the pointers for interacting with the memory. */
    p_memory->read_byte = memory_read_byte;
    p_memory->write_byte = memory_write_byte;
    p_memory->print_stats = memory_print_stats;
    p_memory->reset_stats = memory_reset_stats;
    p_memory->free = memory_free;
}


/* This function implements reads against the memory.  It is a very
 * straightforward implementation; it simply increments the appropriate
 * statistic and then returns the value at the specified address.
 */
unsigned char memory_read_byte(membase_t *mb, addr_t address) {
    memory_t *p_memory = (memory_t *) mb;

    assert(address >= 0 && address < p_memory->mem_size);

#if DEBUG_MEMORY
    printf("Reading memory[%u]\n", address);
#endif

    p_memory->num_reads++;
    return p_memory->mem[address];
}


/* This function implements writes against the memory.  It is a very
 * straightforward implementation; it simply increments the appropriate
 * statistic and then writes to the value at the specified address.
 */
void memory_write_byte(membase_t *mb, addr_t address, unsigned char value) {
    memory_t *p_memory = (memory_t *) mb;

    assert(address >= 0 && address < p_memory->mem_size);

#if DEBUG_MEMORY
    printf("Writing memory[%u] = %u\n", address, value);
#endif

    p_memory->num_writes++;
    p_memory->mem[address] = value;
}


/* This function prints out the statistics for accesses against the memory. */
void memory_print_stats(membase_t *mb) {
    memory_t *p_memory = (memory_t *) mb;

    printf(" * Memory reads=%lld writes=%lld\n",
        p_memory->num_reads, p_memory->num_writes);
}


/* This function resets the statistics for accesses against the memory. */
void memory_reset_stats(membase_t *mb) {
    memory_t *p_memory = (memory_t *) mb;

    p_memory->num_reads = 0;
    p_memory->num_writes = 0;
}


/* This function releases the, uh, memory used by the, uh, memory. */
void memory_free(membase_t *mb) {
    memory_t *p_memory = (memory_t *) mb;
    free(p_memory->mem);
}

