/*============================================================================
 * Declarations of a simple 2D matrix type, along with a few operations that
 * can be performed on matrices.  Matrices can be allocated either from the
 * virtual memory pool using the simple allocator declared in vmalloc.h, or
 * using malloc().
 */


#ifndef MATRIX_H
#define MATRIX_H


/* A simple 2D matrix type. */
typedef struct matrix_t {
    int rows;
    int cols;
    int elems[];
} matrix_t;


matrix_t * malloc_matrix(int rows, int cols);
matrix_t * vmalloc_matrix(int rows, int cols);
void generate_matrix_values(matrix_t *m);
int get_elem(const matrix_t *m, int r, int c);
void set_elem(matrix_t *m, int r, int c, int value);
void multiply_matrices(const matrix_t *m1, const matrix_t *m2,
                       matrix_t *result);
void copy_matrix(const matrix_t *src, matrix_t *dst);
int compare_matrices(const matrix_t *m1, const matrix_t *m2);


#endif /* MATRIX_H */

