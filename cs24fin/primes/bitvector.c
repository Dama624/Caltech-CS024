#include "bitvector.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define UINT_BITS (8 * sizeof(unsigned int))


int init_bitvector(bitvector *v, unsigned int num_bits) {
    unsigned int num_uints;

    assert(v != NULL);

    num_uints = num_bits / UINT_BITS;
    if (num_bits % UINT_BITS != 0)
        num_uints++;

    v->num_bits = num_bits;
    v->num_elems = num_uints;

    v->elems = malloc(num_uints * sizeof(unsigned int));
    if (v->elems != NULL)
        memset(v->elems, 0, num_uints * sizeof(unsigned int));

    return (v->elems != NULL);
}


unsigned int get_bitvalue(bitvector *v, unsigned int index) {
    unsigned int elem;

    assert(v != NULL);
    assert(index < v->num_bits);

    elem = v->elems[index / UINT_BITS];
    elem = (elem >> (index % UINT_BITS)) & 1;

    return elem;
}


void set_bitvalue(bitvector *v, unsigned int index, unsigned int value) {
    unsigned int elem_index, elem, mask;

    assert(v != NULL);
    assert(index < v->num_bits);

    elem_index = index / UINT_BITS;
    mask = 1 << (index % UINT_BITS);

    assert(elem_index < v->num_elems);

    elem = v->elems[elem_index];
    if (value != 0)
        elem = elem | mask;
    else
        elem = elem & ~mask;

    v->elems[elem_index] = elem;
}


void set_all_bits(bitvector *v, unsigned int value) {
    unsigned int elem, i;

    assert(v != NULL);

    if (value)
        elem = (unsigned int) -1;
    else
        elem = 0;

    for (i = 0; i < v->num_elems; i++)
        v->elems[i] = elem;
}


void free_bitvector(bitvector *v) {
    assert(v != NULL);
    free(v->elems);
    v->num_bits = 0;
    v->elems = 0;
}

