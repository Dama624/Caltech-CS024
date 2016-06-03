#ifndef BITVECTOR_H
#define BITVECTOR_H


typedef struct bitvector {
    /* This is the number of unsigned ints in the bit-vector. */
    unsigned int num_elems;

    /* This is the actual number of bits in the bit-vector. */
    unsigned int num_bits;

    unsigned int *elems;
} bitvector;


int init_bitvector(bitvector *v, unsigned int num_bits);

unsigned int get_bitvalue(bitvector *v, unsigned int index);

void set_bitvalue(bitvector *v, unsigned int index, unsigned int value);
void set_all_bits(bitvector *v, unsigned int value);

void free_bitvector(bitvector *v);


#endif /* BITVECTOR_H */

