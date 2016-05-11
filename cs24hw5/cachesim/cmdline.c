#include <stdio.h>
#include <stdlib.h>

#include "cmdline.h"
#include "memory.h"
#include "cache.h"


/* Prints the program usage. */
void usage(const char *progname) {
    printf("usage: %s [cache-spec ...]\n\n", progname);
    printf("\tAll arguments are cache specifications in the form B:S:E, where\n");
    printf("\tB, S and E are all positive integers with the following meanings:\n");
    printf("\t\tB = block size for the cache, in bytes (must be a power of 2)\n");
    printf("\t\tS = the number of cache-sets in the cache (must be a power of 2)\n");
    printf("\t\tE = the number of cache-lines in each cache-set (may be 1 or more)\n");
    printf("\n");
    printf("\tThe actual memory size will be fixed by the program itself, as it\n");
    printf("\tdepends on the specific tests being run against the cache simulator.\n");
}


/* Initializes a set of caches and a memory, using the cache configuration
 * specified from command-line arguments.
 *
 * TODO:  This function doesn't provide any way to clean up the allocated
 *        memory!  Return a pointer to a struct that referenes everything
 *        else, so it can all be deallocated!
 */
membase_t * make_cached_memory(int argc, const char **argv,
                               uint32_t mem_size) {
    int i;
    const char *progname;
    membase_t **p_mems;
    memory_t *p_memory;
    cache_t *p_cache;
    
    progname = argv[0];
    argc--;
    argv++;
    
    p_mems = malloc((argc + 1) * sizeof(membase_t *));

    printf("Constructing memory for simulation (in reverse order):\n");
    
    printf(" * Building memory of size %u bytes\n", mem_size);
    p_memory = malloc(sizeof(memory_t));
    init_memory(p_memory, mem_size);
    p_mems[argc] = (membase_t *) p_memory;
    
    for (i = argc - 1; i >= 0; i--) {
        int block_size, num_sets, lines_per_set;
        int ct = sscanf(argv[i], "%d:%d:%d",
                        &block_size, &num_sets, &lines_per_set);
        if (ct != 3) {
            printf("ERROR:  argument %d isn't correctly formatted.\n", i + 1);
            usage(progname);
            exit(1);
        }
        
        if (block_size <= 0 || !is_power_of_2(block_size)) {
            printf("ERROR:  argument %d:  block size must be a positive "
                   "power of 2, got %d.\n", i + 1, block_size);
            usage(progname);
            exit(1);
        }

        if (num_sets <= 0 || !is_power_of_2(num_sets)) {
            printf("ERROR:  argument %d:  number of cache-sets must be a "
                   "positive power of 2, got %d.\n", i + 1, num_sets);
            usage(progname);
            exit(1);
        }

        if (lines_per_set <= 0) {
            printf("ERROR:  argument %d:  number of cache-lines per set "
                   "must be a positive integer, got %d.\n", i + 1,
                   lines_per_set);
            usage(progname);
            exit(1);
        }

        printf(" * Building cache with a block-size of %d bytes, %d cache-sets,\n"
               "   and %d cache-lines per set.  Total cache size is %d bytes.\n",
               block_size, num_sets, lines_per_set,
               block_size * num_sets * lines_per_set);

        p_cache = malloc(sizeof(cache_t));
        init_cache(p_cache, block_size, num_sets, lines_per_set, p_mems[i + 1]);

        p_mems[i] = (membase_t *) p_cache;
    }
    printf("\n");
    
    return p_mems[0];
}

