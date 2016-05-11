#ifndef CACHE_H
#define CACHE_H


#include "membase.h"


/* This struct represents to a cache line within a cache set. */
typedef struct cacheline_t {
    /* The index of the cache line.  This is mainly for informational and
     * debugging purposes; it is not used in the normal operation of the
     * cache.
     */
    uint32_t line_no;

    /* This value will be 0 if the line is invalid, 1 if it is valid. */
    char valid;

    /* This value will be 0 if the line is clean, 1 if it is dirty. */
    char dirty;

    /* This is the tag for the cache line, taken from the address. */
    uint32_t tag;
    
    /* This is the start of the block of data itself. */
    unsigned char *block;

    /* This value stores the most recent time that the line was accessed*/
    unsigned long long int last_time;
} cacheline_t;


/* This struct represents a cache set within the cache. */
typedef struct cacheset_t {
    /* The number of the cache set.  This allows us to construct addresses
     * of lines within the cache set, when writing them back.
     */
    addr_t set_no;

    /* The number of lines in the cache set.  Obviously, in a given cache,
     * every cache set should have the same # of lines.  We store it here
     * instead of in the cache, so that it's easy to iterate over the lines
     * in the cache.
     */
    int32_t num_lines;

    /* The cache lines in this cache set. */
    cacheline_t *cache_lines;
} cacheset_t;


/* This struct represents a cache that sits in front of another memory (which
 * could be another cache, or the final memory in the sequence), and which
 * will serve requests out of its own cache lines if it can, or will access
 * the next memory in the sequence if a request is not in the cache.
 */
typedef struct cache_t {
    /* The number of reads that occurred at this level of the memory. */
    uint64_t num_reads;
    
    /* The number of writes that occurred at this level of the memory. */
    uint64_t num_writes;

    /* The function to read a byte from the cache. */
    unsigned char (*read_byte)(membase_t *mb, addr_t address);
    
    /* The function to write a byte to the cache. */
    void (*write_byte)(membase_t *mb, addr_t address, unsigned char value);
 
    /* The function to print the cache's access statistics. */
    void (*print_stats)(struct membase_t *mb);
 
    /* The function to reset the cache's access statistics. */
    void (*reset_stats)(struct membase_t *mb);
 
    /* The function to release any internally allocated data used by
     * the memory.
     */
    void (*free)(membase_t *mb);


    /* This is the block size for the cache line, and should be a
     * power of 2.
     */
    uint32_t block_size;


    /* This is the number of address bits devoted to identifying the
     * cache set based on the address.
     */
    uint32_t sets_addr_bits;


    /* This is the number of address bits devoted to the offset within
     * the block based on the address.  It is required to be a power of 2.
     */
    uint32_t block_offset_bits;


    /* The number of cache sets in the cache. */
    uint32_t num_sets;

    /* The array of cache sets themselves. */
    cacheset_t *cache_sets;

    /* The memory that this is a cache of. */
    membase_t *next_memory;

    /* The number of cache hits. */
    uint64_t num_hits;

    /* The number of cache misses. */
    uint64_t num_misses;

} cache_t;


void init_cache(cache_t *p_cache, uint32_t block_size, uint32_t num_sets,
    uint32_t lines_per_set, membase_t *next_mem);

int flush_cache(cache_t *p_cache);


#endif /* CACHE_H */

