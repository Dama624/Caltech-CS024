#ifndef MEMORY_H
#define MEMORY_H


#include "membase.h"


/* This struct holds the state for a simple memory that is an addressable
 * array of bytes.  Thus, the read_byte and write_byte implementations are
 * very simple; they just access or modify values in the array of memory
 * pointed to by mem.  Access statistics and other operations are also
 * provided via the function-pointers held in the struct, which are
 * initialized to point to the memory_t implementations of these functions.
 */
typedef struct memory_t {
    /* The number of reads that occurred at this level of the memory. */
    uint64_t num_reads;

    /* The number of writes that occurred at this level of the memory. */
    uint64_t num_writes;

    /* The function to read a byte from the memory. */
    unsigned char (*read_byte)(membase_t *mb, addr_t address);

    /* The function to write a byte to the memory. */
    void (*write_byte)(membase_t *mb, addr_t address, unsigned char value);

    /* The function to print the memory's access statistics. */
    void (*print_stats)(struct membase_t *mb);

    /* The function to reset the memory's access statistics. */
    void (*reset_stats)(struct membase_t *mb);

    /* The function to release any internally allocated data used by
     * the memory.
     */
    void (*free)(membase_t *mb);

    /* The size of the memory. */
    int32_t mem_size;

    /* The malloc'd region of memory. */
    unsigned char *mem;

} memory_t;


/* Initializes the members of the memory_t struct to be a memory of the
 * specified number of bytes.  This requires a heap allocation, so the
 * allocated memory must be released as well.
 */
void init_memory(memory_t *p_memory, int mem_size);


#endif /* MEMORY_H */
