#ifndef SPARSE_H
#define SPARSE_H

#include <inttypes.h>



typedef struct {
    double d;
    uint32_t rows; // number of rows
    uint32_t cols; // number of columns
    uint32_t nnz;  // number of non-zero elements
    uint32_t *row_ptr; // row pointers
    uint32_t *col_idx; // column indices
    double *values; // list of non-zero values
} Sparse;



// Function prototypes
Sparse* createSparseMatrix(int, int, double);
void freeMatrix(Sparse*);
void appendElement(Sparse*, int, int, double);
void insertElement(Sparse*, int, int, double);
void populateMatrix(Sparse*);
void printRawMatrix(Sparse*);
void printMatrix(Sparse*);
void populateVector(double*, uint32_t);
void csrMatrixVectorMultiply(Sparse*, double*, double*);

#endif // SPARSE_H


