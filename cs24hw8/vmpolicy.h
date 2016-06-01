/*============================================================================
 * This file defines an interface that can be implemented to provide a page
 * replacement policy in the virtual memory system.
 *
 * We don't mind if policies use malloc() and free(), just because it keeps
 * things simpler.
 */

#ifndef VMPOLICY_H
#define VMPOLICY_H

#include "virtualmem.h"

/* Called by vmem_init() to initialize the page replacement policy.  The
 * function should return nonzero for successful initialization, 0 otherwise.
 */
int policy_init(int max_resident);

/* Called by vmem_cleanup() to release any resources used by the policy. */
void policy_cleanup(void);

/* Called by the virtual memory system to inform the policy that a page has been
 * mapped into virtual memory.
 */
void policy_page_mapped(page_t page);

/* Called by the virtual memory system when a SIGALRM signal is fired, so
 * that timer-based policies can update their internal state.
 */
void policy_timer_tick(void);

/* Called by map_page() when space needs to be made for another virtual page
 * to be mapped, by evicting a currently mapped page.  Note that this function
 * both chooses a page to evict, and records in the paging policy that the page
 * is now evicted.  The virtual memory system will also carry through its own
 * tasks for evicting the page.
 */
page_t choose_and_evict_victim_page(void);


#endif /* VMPOLICY_H */

