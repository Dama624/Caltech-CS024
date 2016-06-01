/*============================================================================
 * Implementation of the FIFO page replacement policy.
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

typedef struct loaded_pages_t loaded_pages_t;

/*============================================================================
 * "Loaded Pages" Queue Data Structure
 *
 * This queue data structure records all pages that are currently loaded in
 * the virtual memory, so that we can choose the first page to evict.
 */

struct loaded_pages_t {
    // The data; in this case, a page
    page_t page;
    // The next node
    loaded_pages_t *next;
};


/*============================================================================
 * Policy Implementation
 */


/* Keep track of the head and tail of our queue */
static loaded_pages_t *head;
static loaded_pages_t *tail;
static int max;


/* Initialize the policy.  Return nonzero for success, 0 for failure. */
int policy_init(int max_resident) {
    fprintf(stderr, "Using FIFO eviction policy.\n\n");
    
    head = NULL;
    tail = NULL;
    max = max_resident;
    /* Return nonzero if initialization succeeded. */
    return 1;
}


/* Clean up the data used by the page replacement policy. */
void policy_cleanup(void) {
    // no need for cleanup, init doesn't allocate memory
}


/* This function is called when the virtual memory system maps a page into the
 * virtual address space.  Record that the page is now resident.
 */
void policy_page_mapped(page_t page) {
    // allocate space for a new node on the queue
    loaded_pages_t *node = (loaded_pages_t *)malloc(sizeof(loaded_pages_t));
    node->page = page;
    node->next = NULL;
    // check if this node is the first node in the queue
    if (head == NULL && tail == NULL)
    {
        head = node;
        tail = node;
        return;
    }
    else
    {
        tail->next = node;
        tail = node;
        return;
    }
}


/* This function is called when the virtual memory system has a timer tick. */
void policy_timer_tick(void) {
    /* Do nothing! */
}


/* With a FIFO, we simply evict the page located at the head of the queue.
 */
page_t choose_and_evict_victim_page(void) {
    page_t victim;
    loaded_pages_t *temp;

    // We evict the first member of the queue
    victim = head->page;
    temp = head;
    head = head->next;
    free(temp);
    temp = NULL;

#if VERBOSE
    fprintf(stderr, "Choosing victim page %u to evict.\n", victim);
#endif

    return victim;
}

