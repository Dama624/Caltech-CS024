/*============================================================================
 * This file contains the implementation of the simple user-space virtual
 * memory system.
 */


#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/errno.h>
#include <sys/mman.h>
#include <sys/time.h>

#include "virtualmem.h"
#include "vmpolicy.h"


// For platforms that use MAP_ANON instead of MAP_ANONYMOUS...
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif


/* The start of the virtual address range.  Choosing a value for this is a bit
 * dangerous, because we could hit the memory heap (we are above it) or we
 * could hit shared libraries (we are below them).  However, our program's
 * heap requirements aren't very large, and this value seems to work
 * successfully.
 */
#define VIRTUALMEM_ADDR_START 0x20000000

/* The timer signal is set to trigger on this interval, currently 10ms. */
#define TIMESLICE_SEC 0
#define TIMESLICE_USEC 10000


/*============================================================================
 * Global state for the virtual memory system.
 *
 * (Don't do this at home.  At the least, this should all be wrapped up into
 * a single struct.)
 */

/* This is the address of where the virtual memory range starts. */
static void *vmem_start;

/* This is the address of where the virtual memroy range ends (it is actually
 * one past where virtual memory ends).
 */
static void *vmem_end;


/* The filename of the swap file. */
static char swapfile[40];

/* The file-descriptor of the swap file. */
static int fd_swapfile;


/* The number of pages that are currently resident. */
static unsigned int num_resident;


/* The maximum number of pages that may be resident in memory. */
static unsigned int max_resident;


/* A count of how many faults have occurred since initialization.  Note that
 * this doesn't correspond to the number of page-loads, since we use faults
 * to detect accesses and writes as well.
 */
static unsigned int num_faults;


/* A count of how many page-loads have occurred since initialization. */
static unsigned int num_loads;


/* This page table records the state of every virtual page in the virtual
 * memory area, including whether the page has been mapped into physical
 * memory, and also whether the page has been accessed and/or is dirty.
 */
static pte_t page_table[NUM_PAGES];


/*============================================================================
 * Helper Functions
 */


/* Returns the start of the virtual memory pool. */
void * get_vmem_start() {
    return vmem_start;
}


/* Returns the end of the virtual memory pool. */
void * get_vmem_end() {
    return vmem_end;
}


/* Takes a page number and returns the address of the start of the
 * corresponding virtual memory page.
 */
void * page_to_addr(page_t page) {
    assert(page < NUM_PAGES);
    return vmem_start + page * PAGE_SIZE;
}


/* Takes an address and returns the virtual memory page corresponding to
 * the address.
 */
page_t addr_to_page(void *addr) {
    assert(addr >= vmem_start);
    assert(addr < vmem_end);
    return (addr - vmem_start) / PAGE_SIZE;
}


/* Returns the number of segfaults that occurred in the system.  Note that
 * segfaults don't correspond to page-faults, because we use segfaults for
 * other things besides detecting page faults.  The number of page loads,
 * reported by the next function, is the best one to use to measure the system
 * performance.
 */
unsigned int get_num_faults() {
    return num_faults;
}


/* Returns the number of page-loads (i.e. map_page() calls) that have occurred
 * in the system.  This corresponds directly to the number of "page faults" in
 * the system.  This is the number we want to minimize.
 */
unsigned int get_num_loads() {
    return num_loads;
}


/* Returns a string representation of the signal-code value from the SIGSEGV
 * signal details.
 */
const char *signal_code(int code) {
    switch (code) {
    case SEGV_MAPERR:
        return "SEGV_MAPERR";
    case SEGV_ACCERR:
        return "SEGV_ACCERR";
    default:
        return "UNKNOWN";
    }
}


/*============================================================================
 * Page Table and Page Table Entry (PTE) Helper Functions
 */


/* This function should be used when a page is unmapped, since we want to
 * clear out all bits associated with the page's PTE.
 */
void clear_page_entry(page_t page) {
    assert(page < NUM_PAGES);
    page_table[page] = 0;
}


/* Sets the specified page's "resident" bit in its page-table entry. */
void set_page_resident(page_t page) {
    assert(page < NUM_PAGES);
    page_table[page] |= PAGE_RESIDENT;
}


/* Returns the specified page's "resident" bit.  Nonzero means the page is
 * present in memory, zero means the page is not in virtual memory.
 */
int is_page_resident(page_t page) {
    assert(page < NUM_PAGES);
    return page_table[page] & PAGE_RESIDENT;
}


/* Sets the specified page's "accessed" bit in its page-table entry. */
void set_page_accessed(page_t page) {
    assert(page < NUM_PAGES);
    page_table[page] |= PAGE_ACCESSED;
}


/* Clears the specified page's "accessed" bit in its page-table entry. */
void clear_page_accessed(page_t page) {
    assert(page < NUM_PAGES);
    page_table[page] &= ~PAGE_ACCESSED;
}


/* Returns the specified page's "accessed" bit.  Nonzero means the page has
 * been accessed, zero means the page has not been accessed.
 */
int is_page_accessed(page_t page) {
    assert(page < NUM_PAGES);
    return page_table[page] & PAGE_ACCESSED;
}


/* Sets the specified page's "dirty" bit in its page-table entry. */
void set_page_dirty(page_t page) {
    assert(page < NUM_PAGES);
    page_table[page] |= PAGE_DIRTY;
}


/* Clears the specified page's "dirty" bit in its page-table entry. */
void clear_page_dirty(page_t page) {
    assert(page < NUM_PAGES);
    page_table[page] &= ~PAGE_DIRTY;
}


/* Returns the specified page's "dirty" bit.  Nonzero means the page has
 * been written to, zero means the page has not been written to.
 */
int is_page_dirty(page_t page) {
    assert(page < NUM_PAGES);
    return page_table[page] & PAGE_DIRTY;
}


/* Returns the specified page's permission value from the page-table entry.
 * The other bits (e.g. resident, accessed, dirty) are masked out of this
 * return-value.
 */
int get_page_permission(page_t page) {
    assert(page < NUM_PAGES);
    return page_table[page] & PAGEPERM_MASK;
}


/* Sets the specified page's permission value.  This involves two steps:
 * First, mprotect() is used to set the actual permissions on the page's
 * virtual address range.  After this is completed successfully, the page's
 * Page Table Entry is updated with the new permission value.  (The other bits
 * in the PTE are left unmodified.)
 */
void set_page_permission(page_t page, int perm) {
    assert(page < NUM_PAGES);
    assert(perm == PAGEPERM_NONE || perm == PAGEPERM_READ ||
           perm == PAGEPERM_RDWR);

    /* Call mprotect() to set the memory region's protections. */
    if (mprotect(page_to_addr(page), PAGE_SIZE, pageperm_to_mmap(perm)) == -1) {
        perror("mprotect");
        abort();
    }

    /* Replace old permission with new permission. */
    page_table[page] = (page_table[page] & ~PAGEPERM_MASK) | perm;
}


/* This function converts a page table entry's permission value into the
 * corresponding value to pass to mmap() or mprotect().
 */
int pageperm_to_mmap(int perm) {
    assert(perm == PAGEPERM_NONE || perm == PAGEPERM_READ ||
           perm == PAGEPERM_RDWR);

    switch (perm) {
    case PAGEPERM_NONE:
        return PROT_NONE;

    case PAGEPERM_READ:
        return PROT_READ;

    case PAGEPERM_RDWR:
        return PROT_READ | PROT_WRITE;

    default:
        fprintf(stderr, "pageperm_to_mmap: unrecognized value %u\n", perm);
        abort();
    }
}


/*============================================================================
 * Core Functions for the User-Space Virtual Memory System
 */

/* Declare these functions here, since they really aren't needed outside of
 * the virtual-memory code itself.
 */
void map_page(page_t page, unsigned initial_perm);
void unmap_page(page_t page);
static void sigsegv_handler(int signum, siginfo_t *infop, void *data);
static void sigalrm_handler(int signum, siginfo_t *infop, void *data);


/* This function initializes the virtual memory system with the specified
 * "maximum resident" limit on the number of pages that may be in the address
 * space.  This function does the following:
 *
 * 1)  Set aside a range of addresses for use by the user-space virtual memory
 *     system.
 *
 * 2)  Initialize other virtual memory system state.
 *
 * 3)  Clear the entire Page Table to all 0s.
 *
 * 4)  Initialize the page replacement policy.
 *
 * 5)  Open the swap file /tmp/cs24_pagedev_<pid>, extend it to be the full
 *     size of the virtual address space (NUM_PAGES * PAGE_SIZE), and arrange
 *     for the file to be deleted when the program terminates.
 *
 * 6)  Install the SIGSEGV and SIGALRM handlers.
 *
 * 7)  Start the SIGALRM timer interrupt.
 */
void * vmem_init(unsigned _max_resident) {
    struct sigaction action;
    struct itimerval itimer;

    /* Set up the address range we will use. */
    vmem_start = (void *) VIRTUALMEM_ADDR_START;
    vmem_end = vmem_start + (NUM_PAGES * PAGE_SIZE);

    /* Initialize the values that record how many pages are resident in
     * physical memory, and the maximum number of pages that may be resident.
     */
    num_resident = 0;
    max_resident = _max_resident;
    num_faults = 0;

    fprintf(stderr, "\"Physical memory\" is in the range %p..%p\n * %d pages"
            " total, %d maximum resident pages\n\n", vmem_start, vmem_end,
            NUM_PAGES, max_resident);

    /* Clear the entire page table. */
    memset(page_table, 0, sizeof(page_table));

    /* Initialize the page replacement policy. */
    if (!policy_init(max_resident)) {
        fprintf(stderr, "policy_init: failed to initialize\n");
        abort();
    }

    /* Open the swap file */
    sprintf(swapfile, "/tmp/cs24_pagedev_%05d", getpid());
    fd_swapfile = open(swapfile, O_RDWR | O_CREAT, 0600);
    if (fd_swapfile < 0) {
        perror(swapfile);
        abort();
    }

    /* Immediately unlink it so it will go away when this process terminates */
    if (unlink(swapfile) < 0) {
        perror(swapfile);
        abort();
    }

    /* Extend the file to include the entire address space */

    if (lseek(fd_swapfile, NUM_PAGES * PAGE_SIZE, SEEK_SET) < 0) {
        perror("lseek");
        abort();
    }

    if (write(fd_swapfile, "x", 1) < 1) {
        perror(swapfile);
        abort();
    }

    /* Set up and install the seg-fault signal handler */

    memset(&action, 0, sizeof(action));
    action.sa_sigaction = sigsegv_handler;
    action.sa_flags = SA_SIGINFO | SA_ONSTACK;

    /* Mask timer signals during the SIGSEGV handler */
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGALRM);

    if (sigaction(SIGSEGV, &action, (struct sigaction *) 0) < 0) {
        perror("sigaction(SIGSEGV)");
        exit(1);
    }

    /* Set up and install the timer signal handler */

    memset(&action, 0, sizeof(action));
    action.sa_sigaction = sigalrm_handler;
    action.sa_flags = SA_SIGINFO | SA_ONSTACK;
    if (sigaction(SIGALRM, &action, (struct sigaction *) 0) < 0) {
        perror("sigaction(SIGALRM)");
        exit(1);
    }

    /* Start the periodic timer! */
    itimer.it_interval.tv_sec = TIMESLICE_SEC;
    itimer.it_interval.tv_usec = TIMESLICE_USEC;
    itimer.it_value.tv_sec = TIMESLICE_SEC;
    itimer.it_value.tv_usec = TIMESLICE_USEC;
    if (setitimer(ITIMER_REAL, &itimer, (struct itimerval *) 0) < 0) {
        perror("setitimer");
        exit(1);
    }

    return vmem_start;
}


void vmem_cleanup(void) {
    policy_cleanup();
}


/* This function maps the specified page from the swap file into the virtual
 * address space, and sets up the page permissions so that accesses and writes
 * to the page can be detected.
 */
void map_page(page_t page, unsigned initial_perm) {
    int rc;
    void *input;
    void *virtual;
    assert(page < NUM_PAGES);
    assert(initial_perm == PAGEPERM_NONE || initial_perm == PAGEPERM_READ ||
           initial_perm == PAGEPERM_RDWR);
    assert(!is_page_resident(page));  /* Shouldn't already be mapped */

#if VERBOSE
    fprintf(stderr, "Mapping in page %u.  Resident (before mapping) = %u, "
           "max resident = %u.\n", page, num_resident, max_resident);
#endif

    /* Make sure we don't exceed the physical memory constraint. */
    num_resident++;
    if (num_resident > max_resident) {
        fprintf(stderr, "map_page: exceeded physical memory, resident pages "
                "= %u, max resident = %u\n", num_resident, max_resident);
        abort();
    }

    /* ==== IMPLEMENT =================================================
     *
     * This function must use mmap() to update the process' virtual address
     * space to make the page's address range valid.  After this is done, the
     * function must load the page's contents from the swap file.
     *
     * The mmap() call should use MAP_FIXED | MAP_SHARED | MAP_ANONYMOUS for
     * the flags.  The return-address should be checked for errors (as
     * described in the assignment), and it should also be checked against the
     * input address (they should be the same, or an error should be
     * reported).
     *
     * Once the page is mapped, the function can seek to the start of the
     * page's corresponding slot in the swap-file using lseek(), and the
     * data can be loaded using the read() function.  Make sure to verify the
     * return-values of these functions, and report errors on failure, as
     * described in the assignment.  This will aid in debugging, and it will
     * also avoid unnecessary point-deductions.
     *
     * Finally, the page's Page Table Entry should initialized properly, and
     * the page's permissions should be set appropriately.  (Keep in mind that
     * your read() call will write to the page, so you must tell mmap() to
     * allow reading and writing or else loading the page's contents will
     * fail.)
     */
    /* Updating virtual address space to make page address valid */
    // extract the starting address of page
    input = page_to_addr(page);
    // map page to virtual memory
    virtual = mmap(input, PAGE_SIZE, PROT_READ | PROT_WRITE,
                         MAP_FIXED | MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // check return address for errors
    if (virtual == (void *) -1)
    {
        perror("mmap(from map_page)");
        abort();
    }
    // check return address against input address
    if (input != virtual)
    {
        fprintf(stderr, "virtual memory address != input page address");
        abort();
    }

    /* Loading data from swap */
    // seek to start of corresponding slot in swap file
    if (lseek(fd_swapfile, page * PAGE_SIZE, SEEK_SET) == -1)
    {
        perror("lseek(from map_page)");
        abort();
    }
    // begin reading from the swap
    rc = read(fd_swapfile, virtual, PAGE_SIZE);
    // error handling for read()
    if (rc == -1)
    {
        perror("read(from map_page)");
        abort();
    }
    if (rc != PAGE_SIZE)
    {
        fprintf(stderr, "read: only read %d bytes (%d expected)\n",
                rc, PAGE_SIZE);
        abort();
    }

    /* Updating Page Table Entry */
    // initialize the PTE for the page
    page_table[page] = 0;
    // set page as a current member of the virtual address space
    set_page_resident(page);
    // set page permissions (r/w enabled)
    set_page_permission(page, initial_perm);

    assert(is_page_resident(page));  /* Now it should be mapped! */
    num_loads++;

    /* Inform the paging policy that the page was mapped. */
    policy_page_mapped(page);

#if VERBOSE
    fprintf(stderr, "Successfully mapped in page %u with initial "
        "permission %u.\n  Resident (after mapping) = %u.\n",
        page, initial_perm, num_resident);
#endif
}


/* This function unmaps the specified page from the virtual address space,
 * making sure to write the contents of dirty pages back into the swap file.
 */
void unmap_page(page_t page) {
    int wc;
    void *addr;
    assert(page < NUM_PAGES);
    assert(num_resident > 0);
    assert(is_page_resident(page));

    /* ==== IMPLEMENT =================================================
     *
     * This function must use munmap() to remove the page's address range from
     * the process' virtual address space.
     *
     * Before this can be done, however, the function must check if the page
     * is dirty, and *only* if it is, save the page back to its corresponding
     * slot in the swap file.  (If the function always saves pages back to
     * disk, a significant deduction will be applied.)
     *
     * If the page is dirty, the function can seek to the start of the page's
     * corresponding slot in the swap-file using lseek(), and then use the
     * write() function to save the data into the swap file.  Make sure to
     * verify the return-values of these functions, and report errors on
     * failure, as described in the assignment.  This will aid in debugging,
     * and it will also avoid unnecessary point-deductions.
     *
     * Note that when you are writing the page back to disk, you will be
     * performing a read on the page.  Therefore, you may want to set the
     * page's permissions to allow reading, so that the write() is able to
     * complete successfully.
     *
     * Finally, the page's Page Table Entry should be cleared.
     */
    /* If page is dirty, save page's contents to swap */
    addr = page_to_addr(page);
    if (is_page_dirty(page))
    {
        set_page_permission(page, PAGEPERM_READ);
        // seek to start of page slot in swap
        if (lseek(fd_swapfile, page * PAGE_SIZE, SEEK_SET) == -1)
        {
            perror("lseek(from unmap_page)");
            abort();
        }
        // save page contents to swap
        wc = write(fd_swapfile, addr, PAGE_SIZE);
        // error handling
        if (wc == -1)
        {
            perror("write(from unmap_page");
            abort();
        }
        if (wc != PAGE_SIZE)
        {
            fprintf(stderr, "write: only wrote %d bytes (%d expected)\n",
                    wc, PAGE_SIZE);
            abort();
        }
    } 

    /* Call munmap to remove page address-range from virtual address space */
    if (munmap(addr, PAGE_SIZE) == -1)
    {
        perror("munmap(from unmap_page)");
        abort();
    }

    /* Update Page Table Entry */
    clear_page_entry(page);

    assert(!is_page_resident(page));
    num_resident--;
}


/*============================================================================
 * Signal Handlers for the Virtual Memory System
 */


/* This function is the SIGSEGV handler for the virtual memory system.  If the
 * faulting address is within the user-space virtual memory pool then this
 * function responds appropriately to allow the faulting operation to be
 * retried.  If the faulting address falls outside of the virtual memory pool
 * then the segmentation fault is reported as an error.
 *
 * While a SIGSEGV is being processed, SIGALRM signals are blocked.  Therefore
 * a timer interrupt will never interrupt the segmentation-fault handler.
 */
static void sigsegv_handler(int signum, siginfo_t *infop, void *data) {
    void *addr;
    page_t page;
    int perm;

    /* Only handle SIGSEGVs addresses in range */
    addr = infop->si_addr;
    if (addr < vmem_start || addr >= vmem_end) {
        fprintf(stderr, "segmentation fault at address %p\n", addr);
        abort();
    }

    num_faults++;

    /* Figure out what page generated the fault. */
    page = addr_to_page(addr);
    assert(page < NUM_PAGES);

#if VERBOSE
    fprintf(stderr,
        "================================================================\n");
    fprintf(stderr, "SIGSEGV:  Address %p, Page %u, Code %s (%d)\n",
           addr, page, signal_code(infop->si_code), infop->si_code);
#endif

    /* We really can't handle any other type of code.  On Linux this should be
     * fine though.
     */
    assert(infop->si_code == SEGV_MAPERR || infop->si_code == SEGV_ACCERR);

    /* Map the page into memory so that the fault can be resolved.  Of course,
     * this may result in some other page being unmapped.
     */

    /* ==== IMPLEMENT =================================================
     *
     * Examine the value of infop->si_code to determine if the corresponding
     * page is simply unmapped (SEGV_MAPERR), or if the page is mapped but the
     * access itself was not permitted (SEGV_ACCERR).
     *
     * If the address is unmapped (SEGV_ACCERR), you will need to map the
     * corresponding page into memory.  Make sure to respect the physical
     * memory constraints by evicting a page if there are already max_resident
     * pages loaded.  You can do something like this to evict a page:
     *
     *     assert(num_resident <= max_resident);
     *     if (num_resident == max_resident) {
     *         page_t victim = choose_and_evict_victim_page();
     *         assert(is_page_resident(victim));
     *         unmap_page(victim);
     *         assert(!is_page_resident(victim));
     *     }
     *
     * Keep in mind that the new page will need its Page Table Entry (PTE)
     * and permissions updated properly.
     *
     * If the address is mapped but an access violation occurred (SEGV_ACCERR),
     * you can use the Page Table Entry helper functions to retrieve and update
     * the page's PTE and permissions appropriately.  The set_page_permission()
     * function is particularly useful; it will update the permission values in
     * the page's PTE, as well as modifying the actual virtual-memory page
     * permissions with a call to mprotect().
     *
     * As always, use assertions liberally!  If you have any errors, or you
     * have points in the code that should never be reached, report an error
     * and then call abort() so that your code will fail visibly.  This will
     * greatly aid in debugging.
     */
    /* SEGV_MAPERR */
    if (infop->si_code == SEGV_MAPERR)
    {
        // respect physical memory constraints; evict if max_resident pages
        // loaded.
        assert(num_resident <= max_resident);
        if (num_resident == max_resident) {
            page_t victim = choose_and_evict_victim_page();
            assert(is_page_resident(victim));
            unmap_page(victim);
            assert(!is_page_resident(victim));
        }
        // map corresponding page to memory
        map_page(page, PAGEPERM_NONE);
        assert(is_page_resident(page));
    }
    /* SEGV_ACCERR */
    if (infop->si_code == SEGV_ACCERR)
    {
        assert(is_page_resident(page));
        // check to see what type of access was attempted (read/write)
        perm = get_page_permission(page);
        // read error will occur if page has no read permission
        if (perm == PAGEPERM_NONE)
        {
            // give page read access
            set_page_permission(page, PAGEPERM_READ);
            // page will be read; set page as accessed
            set_page_accessed(page);
            // check to be sure page has read access
            assert(get_page_permission(page) == PAGEPERM_READ);
            // check that the page is set to accessed
            assert(is_page_accessed(page));
        }
        // if it is a write error, handler will be called again, this time
        // with PROT_READ enabled
        else if (perm == PAGEPERM_READ)
        {
            // give page write access
            set_page_permission(page, PAGEPERM_RDWR);
            // page will be written; set page as dirty
            set_page_dirty(page);
            // check to be sure page has write access
            assert(get_page_permission(page) == PAGEPERM_RDWR);
            // check that the page is set to dirty and accessed
            // assert(is_page_accessed(page));
            assert(is_page_dirty(page));
        }
    }
}


/* This function is the SIGALRM handler for the virtual memory system.  On a
 * timer interrupt, call the paging policy timer function.
 *
 * While a SIGSEGV is being processed, SIGALRM signals are blocked.  Therefore
 * a timer interrupt will never interrupt the segmentation-fault handler.
 */
static void sigalrm_handler(int signum, siginfo_t *infop, void *data) {
#if VERBOSE
    fprintf(stderr,
        "================================================================\n");
    fprintf(stderr, "SIGALRM\n");
#endif

    /* All we have to do is inform the page replacement policy that a timer
     * tick occurred!
     */
    policy_timer_tick();
}


