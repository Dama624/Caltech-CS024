/*============================================================================
 * This file contains declarations for the simple user-space virtual memory
 * system.
 */

#ifndef VIRTUALMEM_H
#define VIRTUALMEM_H

#include <stdint.h>


/* When debugging virtual memory, setting this to 1 will result in a lot of
 * details being output to standard error.
 */
#define VERBOSE 1


/* Type for representing a page number.  Since we have a limit of 4K
 * pages, we can use a 16-bit unsigned integer for this value.
 */
typedef uint16_t page_t;

/* Type for representing an entry in the page table.  On IA32, Page Table
 * Entries are 32 bits each, but in our simple virtual memory system, they
 * are only 8 bits because there is no address translation; we only have to
 * store permissions.
 */
typedef unsigned char pte_t;


/* An individual page is 4KiB.  This is a hardware constraint, so we can't
 * change this value.
 */
#define PAGE_SIZE 4096

/* The total number of pages that we will have in our virtual address space.
 * If virtual-memory allocations exceed NUM_PAGES * PAGE_SIZE, the program
 * will crash.
 */
#define NUM_PAGES 4096


/*===========================================================================
 * Page table entry values and helper functions
 */

#define PAGE_RESIDENT 0x01   /* Is the page resident in memory? */
#define PAGE_ACCESSED 0x02   /* Has the page been accessed?     */
#define PAGE_DIRTY    0x04   /* Has the page been modified?     */

#define PAGEPERM_MASK 0xF0   /* A mask for extracting the permission value. */

#define PAGEPERM_NONE 0x10   /* No access is permitted.         */
#define PAGEPERM_READ 0x20   /* Read-only access is permitted.  */
#define PAGEPERM_RDWR 0x40   /* Read/write access is permitted. */


/* Functions for manipulating entries in the page table. */
void clear_page_entry(page_t page);
void set_page_resident(page_t page);
int is_page_resident(page_t page);
void set_page_accessed(page_t page);
void clear_page_accessed(page_t page);
int is_page_accessed(page_t page);
void set_page_dirty(page_t page);
void clear_page_dirty(page_t page);
int is_page_dirty(page_t page);
int get_page_permission(page_t page);
void set_page_permission(page_t page, int perm);

/* This function translates permission values from page-table entries into the
 * corresponding permissions for mmap() and mprotect() to use.
 */
int pageperm_to_mmap(int perm);

/*============================================================================
 * Functions for the virtual memory system
 */

/* Start the virtual memory manager.  This returns the base virtual address. */
void * vmem_init(unsigned int max_resident);

/* Functions to determine the start and end of the virtual memory area, and
 * to map between addresses and pages.
 */
void * get_vmem_start();
void * get_vmem_end();
void * page_to_addr(page_t page);
page_t addr_to_page(void *addr);

/* Return statistics about the virtual memory system. */
unsigned int get_num_faults();
unsigned int get_num_loads();

#endif /* VIRTUALMEM_H */
