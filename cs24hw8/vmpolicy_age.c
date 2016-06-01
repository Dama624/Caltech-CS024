/*============================================================================
 * Implementation of the AGING page replacement policy.
 *
 * We don't mind if paging policies use malloc() and free(), just because it
 * keeps things simpler.  In real life, the pager would use the kernel memory
 * allocator to manage data structures, and it would endeavor to allocate and
 * release as little memory as possible to avoid making the kernel slow and
 * prone to memory fragmentation issues.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "vmpolicy.h"


typedef struct page_age_t page_age_t;

/*============================================================================
 * "Loaded Pages" Data Structure
 *
 * This data structure records all pages that are currently loaded in the
 * virtual memory, so that we can choose a random page to evict very easily.
 */

typedef struct loaded_pages_t {
    /* The maximum number of pages that can be resident in memory at once. */
    int max_resident;
    
    /* The number of pages that are currently loaded.  This can initially be
     * less than max_resident.
     */
    int num_loaded;
    
    /* This is the array of pages that are actually loaded.  Note that only the
     * first "num_loaded" entries are actually valid.
     */
    page_age_t *pages[];
} loaded_pages_t;


// A data structure containing a page and its age
struct page_age_t {
    // the page
    page_t page;
    // its age
    uint16_t age;
};

/*============================================================================
 * Policy Implementation
 */


/* The list of pages that are currently resident. */
static loaded_pages_t *loaded;


/* Initialize the policy.  Return nonzero for success, 0 for failure. */
int policy_init(int max_resident) {
    fprintf(stderr, "Using RANDOM eviction policy.\n\n");
    
    loaded = malloc(sizeof(loaded_pages_t) + max_resident * sizeof(page_age_t));
    if (loaded) {
        loaded->max_resident = max_resident;
        loaded->num_loaded = 0;
    }
    
    /* Return nonzero if initialization succeeded. */
    return (loaded != NULL);
}


/* Clean up the data used by the page replacement policy. */
void policy_cleanup(void) {
    int i;

    // need to free page array elements if there exist any
    if (loaded->num_loaded != 0)
    {
        for (i = 0; i < loaded->num_loaded; i++)
        {
            free(loaded->pages[i]);
        }
    }
    free(loaded);
    loaded = NULL;
}


/* This function is called when the virtual memory system maps a page into the
 * virtual address space.  Record that the page is now resident.
 */
void policy_page_mapped(page_t page) {
    page_age_t *new;

    assert(loaded->num_loaded < loaded->max_resident);
    new = (page_age_t *)malloc(sizeof(page_age_t));
    new->page = page;
    new->age = 0x8000;
    loaded->pages[loaded->num_loaded] = new;
    loaded->num_loaded++;
}


/* This function is called when the virtual memory system has a timer tick. */
void policy_timer_tick(void) {
    int i;

    // traverse all pages
    for (i = 0; i < loaded->num_loaded; i++)
    {
        // shift page age right (logical) by 1 bit
        (loaded->pages[i])->age >>= 1;
        // topmost bit is 0; set to 1 if page has been accessed
        if (is_page_accessed((loaded->pages[i])->page))
        {
            (loaded->pages[i])->age |= 0x8000;
            // clear accessed bit
            clear_page_accessed((loaded->pages[i])->page);
        }
    }
}


/* Choose a page to evict from the collection of mapped pages.  Then, record
 * that it is evicted.  This is very simple since we are implementing a random
 * page-replacement policy.
 */
page_t choose_and_evict_victim_page(void) {
    int i_victim;
    page_t victim;
    int i;
    uint16_t min_age;

    /* Figure out which page to evict. */
    min_age = 0xFFFF;
    i_victim = 0;
    for (i = 0; i < loaded->num_loaded; i++)
    {
        if ((loaded->pages[i])->age < min_age)
        {
            i_victim = i;
            min_age = (loaded->pages[i])->age;
        }
    }
    victim = loaded->pages[i_victim]->page;

    /* Shrink the collection of loaded pages now, by moving the last page in the
     * collection into the spot that the victim just occupied.
     */
    loaded->num_loaded--;
    free(loaded->pages[i_victim]);
    loaded->pages[i_victim] = loaded->pages[loaded->num_loaded];

#if VERBOSE
    fprintf(stderr, "Choosing victim page %u to evict.\n", victim);
#endif

    return victim;
}

