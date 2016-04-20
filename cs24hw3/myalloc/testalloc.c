/*! \file
 * This is a relatively sophisticated memory-allocator tester to record a
 * sequence of allocations and deallocations, so that they can be replayed to
 * the allocator and the responses analyzed.
 *
 * Adapted from Andre DeHon's CS24 2004, 2006 material.
 * Copyright (C) California Institute of Technology, 2004-2009.
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "errno.h"
#include "myalloc.h"
#include "sequence.h"

#define VERBOSE 0

// some random numbers...

int random_int(int max) {
  int rnd;
  int result;
  rnd = rand();
  result = 1 + (int) ((long long) max * (long long) rnd / ((long long) RAND_MAX + 1.0));
  // debug
  //printf("random_int rnd=%x max=%d result=%d\n",rnd,max,result);

  return result;
}


int random_block_size(int max_value) {

  // blah, almost certainly not a good model of
  //  typical allocations, but workable for a crude test
  return random_int(max_value / 4);

}

int random_byte() {
  return random_int(256) - 1;
}

// fill in block p2 of length len with data from p1
void fill_data(unsigned char *p1, unsigned char *p2, int len) {
  int i;
  unsigned char *ptr1, *ptr2;
  ptr1 = p1;
  ptr2 = p2;

  if (VERBOSE) {   // very verbose (for debugging)
    printf("now filling %x from %x to length %d\n",
           (uintptr_t) p2, (uintptr_t) p1, len);
  }

  for (i = 0; i < len; i++) {
    *ptr2 = *ptr1;
    if (VERBOSE) {    // very verbose
      printf("now writing %x<-%u (from %x)\n",
             (uintptr_t) ptr2, *ptr2, (uintptr_t) ptr1);
    }
    ptr1++;
    ptr2++;
  }
}


// try applying sequence
int try_sequence(SEQLIST *test_sequence, int mem_size) {
  SEQLIST *sptr;
  unsigned char *mblock;

  // reset the memory allocator being tested
  MEMORY_SIZE = mem_size;
  init_myalloc();

  for (sptr = test_sequence; !seq_null(sptr); sptr = seq_next(sptr)) {
    if (seq_alloc(sptr)) {     // allocate a block
      mblock = myalloc(seq_size(sptr));
      if (mblock == 0) {
        return 0; // failed -- return indication
      }
      else {
        // keep track of address allocated (for later frees)
        seq_set_myalloc_block(sptr, mblock);
        // put data in the block
        //  (so we can test that it holds data w/out corruption)
        fill_data(seq_ref_block(sptr), mblock, seq_size(sptr));
      }
    }
    else {    // dealloc
      myfree(seq_myalloc_block(seq_tofree(sptr)));
    }
  }

  return 1; // succeeded in allocating entire sequence
}


// search over memory sizes between low and high
//  report smallest size that can accommodate the sequence
int binary_search_required_memory(SEQLIST *test_sequence, int low, int high) {
  // invariant: low not achievable, high is achievable

  int mid;

  if (low + 1 == high) {     // nothing in between, we've found the smallest
    return high;
  }
  else {
    mid = (low + high + 1) / 2;
    if (try_sequence(test_sequence, mid)) {
      if (VERBOSE)
        printf("\tSucceeded for %d\n", mid);

      return(binary_search_required_memory(test_sequence, low, mid));
    }
    else {
      if (VERBOSE)
        printf("\tFailed for %d\n", mid);

      return(binary_search_required_memory(test_sequence, mid, high));
    }
  }
}


// allocate (from normal malloc) a block of size blocks
//  and put data into it
// This supports data integrity tests.
unsigned char *allocate_and_fill(int size) {
  unsigned char *result;
  unsigned char *ptr;

  result = (unsigned char *) malloc(size);
  if (result == (unsigned char *) 0) {
    fprintf(stderr,"real memory system out of memory.\n");
    abort();
  }

  if (VERBOSE) {
      printf("ref fill for %x\n", (uintptr_t) result);
  }

  for (ptr = result; ptr < result + size; ptr++) {
    *ptr = random_byte();
    if (VERBOSE) {     // very
      printf("\tputting %u in %x\n", *ptr, (uintptr_t) ptr);
    }
  }

  return result;
}


// check if p1 and p2 contain the same data up to length len
// This is used to check buffer has not been corrupted
int same_data(unsigned char *p1, unsigned char *p2, int len) {
  int i;
  int result;
  unsigned char *ptr1, *ptr2;

  ptr1 = p1;
  ptr2 = p2;

  result = 1;

  for (i = 0; i < len; i++) {
    if (*ptr1 != *ptr2) {
      if (VERBOSE) {
        printf("error at %x (%d/%d): got %u expect %u (from %x base %x)\n",
               (uintptr_t) ptr2, i, len, *ptr2, *ptr1, (uintptr_t) ptr1,
               (uintptr_t) p1);
      }
      result = 0;
    }
    ptr1++;
    ptr2++;
  }

  return result;
}


// check all still allocated blocks in a test sequence
//  contain the data originally placed into them
//  i.e. have not been corrupted
int check_data(SEQLIST *test_sequence) {
  int result;
  SEQLIST *current;

  result = 0; // stays zero if no errors

  for (current = test_sequence; !seq_null(current); current = seq_next(current)) {
    // only check if an allocate which has not been freed
    if (seq_alloc(current) && !seq_freed(current)) {
      if (!same_data(seq_ref_block(current), seq_myalloc_block(current),
                     seq_size(current))) {
        if (VERBOSE) {
          printf("Mismatch in sequence starting at:\n");
          seq_print(current);
        }

        // returning a 1 means it failed
        result = 1;
      }
    }
  }

  return result;
}


// create a test sequence which never uses more than max_used_memory
//   and allocates a total of max_used_memory*allocation_factor
SEQLIST *generate_sequence(int max_used_memory, int allocation_factor) {
  int used_memory = 0;
  int total_allocated = 0;
  int next_block_size = 0;
  int allocated_blocks = 0;
  int actual_max_used_memory = 0;

  SEQLIST *test_sequence = (SEQLIST *) 0;
  SEQLIST *tail_sequence = (SEQLIST *) 0;

  unsigned char *new_block_ref;

  while (total_allocated < allocation_factor * max_used_memory) {
    next_block_size = random_block_size(max_used_memory);

    // first see if we need to free anything in order to
    //  accommodate the new allocation
    while (used_memory + next_block_size > max_used_memory) {
      // randomly pick a block to free
      SEQLIST *tofree =
        find_nth_allocated_block(test_sequence, random_int(allocated_blocks));

      // add the free
      tail_sequence = seq_set_next_free(tofree, tail_sequence);

      // reclaim the memory
      used_memory -= seq_size(tofree);
      allocated_blocks--;

      // mark the old block as something that has been freed
      seq_free(tofree);
    }

    // allocate a reference buffer for the new block
    new_block_ref = allocate_and_fill(next_block_size);

    // now allocate that block
    if (seq_null(test_sequence)) {
      // special case for first allocation
      test_sequence = seq_add_front(next_block_size, new_block_ref, (SEQLIST *) 0);
      tail_sequence = test_sequence;
    }
    else {
      // typical case we add at the end
      tail_sequence =
        seq_set_next_allocate(next_block_size, new_block_ref, tail_sequence);
    }

    // debug
    //seq_print(tail_sequence); // just prints the new one

    total_allocated += next_block_size;
    used_memory += next_block_size;

    if (used_memory > actual_max_used_memory)
      actual_max_used_memory = used_memory;

    allocated_blocks++;
  }

  // just so can manually see this is doing something sensible
  printf("Actual maximum memory usage %d (%f)\n", actual_max_used_memory,
         ((double) actual_max_used_memory / (double) max_used_memory));

  return test_sequence;
}



int main(int argc, char *argv[]) {

  int max_used_memory;
  int allocation_factor;
  int memory_required;

  SEQLIST *test_sequence;

  max_used_memory = 2000;
  allocation_factor = 11;

  printf("running with MAX_USED_MEMORY=%d and ALLOCATION_FACTOR=%d\n",
         max_used_memory, allocation_factor);

  test_sequence = generate_sequence(max_used_memory, allocation_factor);
  if (VERBOSE)
    seq_print(test_sequence);

  // check that allocation can actually do something.
  // This becomes upper bound on binary search.
  if (try_sequence(test_sequence, max_used_memory * allocation_factor * 2)) {

    // binary search for smallest MEMORY_SIZE which can accommodate
    memory_required = binary_search_required_memory(test_sequence,
      max_used_memory - 1, max_used_memory * allocation_factor * 2);

    // run it one more time at the identified size.
    // this makes sure that the data is set from a successful run.
    if (try_sequence(test_sequence, memory_required)) {
      // check if data contents are intact
      if (check_data(test_sequence)) {
        printf("Data integrity FAIL.\n");
      }
      else {
        printf("Data integrity PASS.\n");
      }

      // print statistics
      printf("Memory utilization: (%d/%d)=%f\n", max_used_memory, memory_required,
             ((double) max_used_memory / (double) memory_required));
    }
    else {
      printf("Consistency problem: binary_search_reqruired_memory "
             "returned %d, but final test failed\n", memory_required);
    }
  }
  else {
    printf("Requires more memory than the no-free case.\n");
  }
  return 1;
}


