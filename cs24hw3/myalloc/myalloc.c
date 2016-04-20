/*! \file
 * Implementation of a simple memory allocator.  The allocator manages a small
 * pool of memory, provides memory chunks on request, and reintegrates freed
 * memory back into the pool.
 *
 * Adapted from Andre DeHon's CS24 2004, 2006 material.
 * Copyright (C) California Institute of Technology, 2004-2010.
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>

#include "myalloc.h"


/*!
 * These variables are used to specify the size and address of the memory pool
 * that the simple allocator works against.  The memory pool is allocated within
 * init_myalloc(), and then myalloc() and free() work against this pool of
 * memory that mem points to.
 */
int MEMORY_SIZE;
unsigned char *mem;


/* Blocks are represented with a signed int header indicating the block's 
 * size (positive if free, negative if allocated) and a footer, identical 
 * to the header. We represent both here through a struct named "tag".
 */
typedef struct {
    // Negative size = allocated
    // Positive size = free
    int size;
} tag;
tag *start; // start of the memory pool (points to header)
tag *end; // end of the memory pool (points to footer)
tag *point_to_foot(tag* head){
    // Returns a pointer to the start of the footer tag of a block given a 
    // pointer to the start of the block's header tag
    return (tag *) ((void *) head + sizeof(tag) + abs(head->size));
}
tag *point_to_head(tag* foot){
    // Returns a pointer to the start of the head tag of a block given a 
    // pointer to the start of the block's foot tag
    return (tag *) ((void *) foot - abs(foot->size) - sizeof(tag));
}  

/*!
 * This function initializes both the allocator state, and the memory pool.  It
 * must be called before myalloc() or myfree() will work at all.
 *
 * Note that we allocate the entire memory pool using malloc().  This is so we
 * can create different memory-pool sizes for testing.  Obviously, in a real
 * allocator, this memory pool would either be a fixed memory region, or the
 * allocator would request a memory region from the operating system (see the
 * C standard function sbrk(), for example).
 */
void init_myalloc() {
    tag * h;
    tag * f;

    /*
     * Allocate the entire memory pool, from which our simple allocator will
     * serve allocation requests.
     */
    mem = (unsigned char *) malloc(MEMORY_SIZE);
    if (mem == 0) {
        fprintf(stderr,
                "init_myalloc: could not get %d bytes from the system\n",
                MEMORY_SIZE);
        abort();
    }

    /* You can initialize the initial state of your memory pool here. 
     *
     * Upon initializing, there is a single block of free memory with enough
     * space for MEMORY_SIZE - 2 * 4 bytes (signed int header/footer).
     */
    tag init = {MEMORY_SIZE - (2 * sizeof(tag))};
    // Set header
    h = (tag *) mem;
    *h = init;
    // Set footer
    f = point_to_foot(h);
    *f = init;
    // Set payload pointer
    start = h;
    end = f;
}


/*!
 * Attempt to allocate a chunk of memory of "size" bytes.  Return 0 if
 * allocation fails.
 * Due to first-fit placement strategy, this operation takes O(n)
 */
unsigned char *myalloc(int size) {
    int free_block_size; // temporary variable
    tag * temp_ptr;
    tag * min; // pointer to smallest free block > size
    tag temp_tag = {size * -1}; // temporary header/footer indicating size

    /* Placement strategy: first-fit; Find the first free block > size */
    min = start; // initial min points to start of memory pool
    temp_ptr = point_to_foot(min); // temp pointer points to foot of 1st block
    while (temp_ptr != end)
    {
        temp_ptr++;
        if (temp_ptr->size == size || temp_ptr->size > size + 7)
        {
            min = temp_ptr;
            break;
        }
        temp_ptr = point_to_foot(temp_ptr);
    }
    
    /* Failed attempt at best-fit; not sure why this doesn't work? Utilization
     * ~0.10
     */
    //
    // while (temp_ptr != end) // loop until temp pointer points to foot of pool
    // {
    //     temp_ptr++; // temp pointer now points to head of next block
    //     if (temp_ptr->size == size) // optimal case
    //     {
    //         min = temp_ptr;
    //         break;
    //     }
    //     else if (temp_ptr->size > size + 7 &&
    //              min->size < 0)
    //     {
    //         min = temp_ptr;
    //     }
    //     else if (temp_ptr->size > size + 7 &&
    //              min->size > size + 7)
    //     {
    //         if (temp_ptr->size < min->size)
    //         {
    //             min = temp_ptr;
    //         }
    //     }
    //     temp_ptr = point_to_foot(temp_ptr);
    //     // temp pointer points to foot of next block
    // }
    // min is either the head of last block or head of smallest free block
    // greater than size

    // Pre-emptively store pointer to payload
    unsigned char *resultptr = (unsigned char *) (min + 1);

    if (size == min->size) {
        // special case in which we can allocate our entire block
        min->size *= -1;
        point_to_foot(min)->size *= 1;
        return resultptr;
    }
    /* size + 8 bytes ensures header and footer of resulting blocks
     * after split can be made, and the remaining free block contains
     * at least 0 bytes free space. */
    else if (size + 7 < min->size) {
        /* Perform block splitting */
        // Keep track of size of free block
        free_block_size = min->size;
        // Update free block header's tag to new size
        *min = temp_tag;
        // Add footer tag
        *point_to_foot(min) = temp_tag;
        // Add header tag for the new free block
        min = point_to_foot(min) + 1;
        temp_tag.size = free_block_size - size - (2 * sizeof(tag));
        *min = temp_tag;
        // update free block footer tag to new free size
        *point_to_foot(min) = temp_tag;
        return resultptr;
    }
    else {
        fprintf(stderr, "myalloc: cannot service request of size %d with"
                " %d bytes allocated\n", size, (min->size));
        return (unsigned char *) 0;
    }
}


/*!
 * Free a previously allocated pointer.  oldptr should be an address returned by
 * myalloc().
 * Coalescing only looks ahead and before the pointer, and so should take O(1)
 */
void myfree(unsigned char *oldptr) {
    tag * current_head; // points to head of block containing oldptr payload
    tag * next_head; // points to head of block after oldptr block
    tag * prev_foot; // points to tail of block before olptr block
    int sum; // temporary storage value
    int forward = 0; // flag for whether forward coalescing occurred

    /* Simply mark the block as free */
    // head of block containing oldptr paylod is oldptr - sizeof(tag)
    current_head = (tag *) (oldptr - sizeof(tag)); // start from head
    // First check if the block needs to be free
    if (current_head->size < 0)
    {
        current_head->size *= -1;
        point_to_foot(current_head)->size *= -1;
    }
    else
    {
        // Block is already free
        fprintf(stderr, "myalloc: cannot free an already free block\n");
    }

    /* Forward coalescing */
    next_head = point_to_foot(current_head) + 1;
    if (next_head != (end + 1)) // Check if not at the end of memory pool
    {
        if (next_head->size >= 0) // Check if block ahead is free
        {
            // Begin coalescing by updating current head
            sum = current_head->size + next_head->size + (2 * sizeof(tag));
            current_head->size = sum;
            // Update next head's foot
            point_to_foot(next_head)->size = sum;
            // Update forward coalesce flag
            forward = 1;
        }
    }

    /* Reverse coalescing */
    // Check if current block is not the first block
    if (current_head != start)
    {
        prev_foot = current_head - 1;
        // Check if block behind is free
        if ((current_head - 1)->size >= 0)
        {
            sum = prev_foot->size + current_head->size + (2 * sizeof(tag));
            // Check forward coalesce flag
            if (forward)
            {
                // Forward coalesced; foot is next_head's foot
                point_to_foot(next_head)->size = sum;
            }
            else
            {
                // No forward coalescing; foot is current_head's foot
                point_to_foot(current_head)->size = sum;
            }
            // Update prev_head's value
            point_to_head(prev_foot)->size = sum;
        }
    }
}
