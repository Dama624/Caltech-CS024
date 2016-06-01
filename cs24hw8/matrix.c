/*============================================================================
 * Implementation of a simple 2D matrix type, along with a few operations that
 * can be performed on matrices.  Matrices are allocated from the virtual
 * memory pool using the simple allocator declared in vmalloc.h.
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "vmalloc.h"


/* Allocate a new matrix object of size rows x cols, from the virtual memory
 * pool.  The elements themselves are uninitialized.
 */
matrix_t * vmalloc_matrix(int rows, int cols) {
    matrix_t *m;

    m = vmem_alloc(sizeof(matrix_t) + rows * cols * sizeof(int));
    m->rows = rows;
    m->cols = cols;

    return m;
}


/* Allocate a new matrix object of size rows x cols, using malloc().  The
 * elements themselves are uninitialized.  This function allows us to have a
 * copy of the test matrices outside of the virtual memory system, so that we
 * can verify that no contents are lost or corrupted by bugs in the virtual
 * memory system.
 */
matrix_t * malloc_matrix(int rows, int cols) {
    matrix_t *m;

    m = malloc(sizeof(matrix_t) + rows * cols * sizeof(int));
    m->rows = rows;
    m->cols = cols;
    return m;
}


/* Generate random values in the range [-1000, 1000] for the specified
 * matrix.
 */
void generate_matrix_values(matrix_t *m) {
    int i;

    assert(m != NULL);

    for (i = 0; i < m->rows * m->cols; i++)
        m->elems[i] = rand() % 2001 - 1000;
}


/* Returns the element at the specified row and column in the matrix. */
int get_elem(const matrix_t *m, int r, int c) {
    int index;
    assert(m != NULL);
    assert(r >= 0 && r < m->rows);
    assert(c >= 0 && c < m->cols);

    index = r * m->cols + c;
    assert(index >= 0 && index < m->rows * m->cols);
    return m->elems[index];
}


/* Sets the element at the specified row and column in the matrix. */
void set_elem(matrix_t *m, int r, int c, int value) {
    int index;
    assert(m != NULL);
    assert(r >= 0 && r < m->rows);
    assert(c >= 0 && c < m->cols);

    index = r * m->cols + c;
    assert(index >= 0 && index < m->rows * m->cols);
    m->elems[index] = value;
}


/* Multiplies the two matrices m1 and m2, storing the results into the result
 * matrix.
 */
void multiply_matrices(const matrix_t *m1, const matrix_t *m2,
                       matrix_t *result) {
    int r, c, i, val;

    assert(m1 != NULL);
    assert(m2 != NULL);
    assert(result != NULL);

    assert(m1->cols == m2->rows);
    assert(m1->rows == result->rows);
    assert(m2->cols == result->cols);

    for (r = 0; r < result->rows; r++) {
        printf(".");
        fflush(stdout);
        for (c = 0; c < result->cols; c++) {
            val = 0;
            for (i = 0; i < m1->cols; i++)
                val += get_elem(m1, r, i) * get_elem(m2, i, c);

            set_elem(result, r, c, val);
        }
    }
    printf("\n");
}


/* Given two matrices of the same dimensions, copies the elements from the
 * source matrix into the destination matrix.
 */
void copy_matrix(const matrix_t *src, matrix_t *dst) {
    int i;

    assert(src != NULL);
    assert(dst != NULL);
    assert(src->rows == dst->rows);
    assert(src->cols == dst->cols);

    for (i = 0; i < src->rows * src->cols; i++)
        dst->elems[i] = src->elems[i];
}


/* Compare two matrices for equality; returns nonzero if the matrices have the
 * same values, or zero if the matrices are different.
 */
int compare_matrices(const matrix_t *m1, const matrix_t *m2) {
    int i;

    assert(m1 != NULL);
    assert(m2 != NULL);

    if (m1->rows != m2->rows || m1->cols != m2->cols)
        return 0;

    for (i = 0; i < m1->rows * m1->cols; i++) {
        if (m1->elems[i] != m2->elems[i])
            return 0;
    }

    return 1;
}

