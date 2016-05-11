#ifndef __HEAP_H__
#define __HEAP_H__


#include "membase.h"


/* A simple heap data structure, for storing floats. */
typedef struct {
    /* Number of values currently in the heap. */
    int num_values;

    /* The maximum number of values to be stored in the heap. */
    int max_values;

    /* The values in the heap. */
    membase_t *memory;
} float_heap;


/* Initialize a heap data structure. */
void init_heap(float_heap *p_heap, membase_t *memory, int max_values);

/* Returns the first (i.e. smallest) value in the heap. */
float get_first_value(float_heap *p_heap);

/* Adds another value to the heap, in the proper place. */
void add_value(float_heap *p_heap, float newval);

#endif /* __HEAP_H__ */

