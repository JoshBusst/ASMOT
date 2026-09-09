#ifndef SPARSE_H
#define SPARSE_H
#define ULONG_MAX 12
#include <inttypes.h>



typedef struct {
    double d; // target density
    uint32_t rows; // number of rows
    uint32_t cols; // number of columns
    uint32_t nnz;  // number of non-zero elements
    uint32_t *row_ptr; // row pointers
    uint32_t *col_idx; // column indices
    double *values; // list of non-zero values
} Matrix;

typedef struct {
    struct Matrix base;
    int csrout;
} CSR;



// Function prototypes
Matrix* createSparseMatrix(int, int, double);
void freeMatrix(Matrix*);
void appendElement(Matrix*, int, int, double);
void insertElement(Matrix*, int, int, double);
void populateMatrix(Matrix*);
void printRawMatrix(Matrix*);
void printMatrix(Matrix*);
void populateVector(double*, uint32_t);
void csrMatrixVectorMultiply(Matrix*, double*, double*);

#endif // SPARSE_H


