#include "sparse.h"

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include "lib.h"

#define TIMELOCAL 1


// Function to create a sparse matrix in CSR format
Sparse* createSparseMatrix(int rows, int cols, double d) {
    assert((rows < 2e6) && (cols < 2e6));
    assert(d < 1);

    int key;
    if(TIMELOCAL)
    {
        printf("Generating matrix...\n");

        // Start timing function
        key = tic();
    }

    uint64_t nnz = (uint64_t)(rows*(cols*d)); // Number of non-zero elements

    float matrix_memory = sizeof(Sparse)/1e9;
    Sparse* matrix = (Sparse*)malloc(matrix_memory);

    if (matrix == NULL)
    {
        char msg[60];
        sprintf(msg, "Unable to allocate %.2f Gb of memory to generate matrix", matrix_memory);
        perror(msg);
        exit(1);
    }

    uint64_t memory = ((rows + nnz + 1) * sizeof(uint32_t) + nnz*sizeof(double));

    if(memory > ULONG_MAX)
    {
        printf("Unable to populate matrix. Number of required bytes exceeds ULONG_MAX.\n");
        printf("Parameters larger than ULONG_MAX lead to undefined malloc() behaviour.\n");
        
        return matrix;
    }

    matrix->d = d;
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->nnz = 0;
    matrix->row_ptr = (uint32_t*)malloc((rows + 1) * sizeof(uint32_t));
    matrix->col_idx = (uint32_t*)malloc(nnz * sizeof(uint32_t));
    matrix->values = (double*)malloc(nnz * sizeof(double));
        
    if (matrix->row_ptr == NULL || matrix->col_idx == NULL || matrix->values == NULL)
    {
        char msg[60];
        sprintf(msg, "Unable to allocate %.2lf Gb of memory to populate matrix\n", (float)(memory/1e9));
        perror(msg);
        exit(1);
    }

    // Initialize row_ptr with zeros
    for (int i = 0; i <= rows; i++) {
        matrix->row_ptr[i] = 0;
    }
    
    if(TIMELOCAL)
    {
        printf("Successfully allocated %.6lf Gb of memory to populate matrix.\n", (float)(memory/1e9));
        // Stop timing function
        toc("Matrix was generated in", key);
    }

    return matrix;
}



// Unallocates memory held by a sparse matrix
void freeMatrix(Sparse* matrix)
{
    printf("Freeing matrix...\n\n");
    free(matrix->row_ptr);
    free(matrix->col_idx);
    free(matrix->values);
    free(matrix);
}



// extremely damaging to the structure of the matrix. Use only when
// generating matrices and not otherwise
void appendElement(Sparse* matrix, int row, int col, double value) {
    if (row < 0 || row >= matrix->rows || col < 0 || col >= matrix->cols) {
        fprintf(stderr, "Invalid row or column index\n");
        return;
    }

    matrix->values[matrix->nnz] = value;
    matrix->col_idx[matrix->nnz] = col;
    matrix->nnz++;

    // don't need to increment succeeding row pointers. relies on future appendElement()
    // calls to set row pointers
    if(row + 1 < matrix->rows)
    {
        matrix->row_ptr[row + 1] = matrix->nnz;
    }
}



// Function to set a non-zero element in the matrix
void insertElement(Sparse* matrix, int row, int col, double value) {
    if (row < 0 || row >= matrix->rows || col < 0 || col >= matrix->cols)
    {
        fprintf(stderr, "Invalid row or column index\n");
        return;
    }

    uint32_t index;
    if(row == matrix->rows - 1)
    {
        index = matrix->nnz; // end of insert window
    }
    else
    {
        index = matrix->row_ptr[row + 1];
    }

    
    // Insert element at col index. Update nnz
    insertint32(matrix->col_idx, matrix->nnz, index, col);
    insertdouble(matrix->values, matrix->nnz, index, value);
    matrix->nnz++;


    // increment succeeding row pointers
    for(uint32_t i=row + 1; i<matrix->rows; i++)
    {
        matrix->row_ptr[i]++;
    }
}



//This function populates an empty sparse matrix with up tp nnz elements
void populateMatrix(Sparse* matrix)
{
    uint32_t nnz = matrix->rows*(matrix->cols*matrix->d);
    int step = (int)(2/matrix->d); // the 2 accounts for probability distriubtion

    if(nnz < 1)
    {
        printf("Unable to populate matrix with 0 values or less.\n");
        return;
    }

    uint32_t row = 0, col = 0, valuesSet = 0;
    uint32_t rowmax = matrix->rows;
    uint32_t colmax = matrix->cols;
    uint32_t updateDelay = (int)(nnz/100);

    if(updateDelay < 1) { updateDelay = 1; }

    assert(rowmax > 0);
    assert(colmax > 0);

    int key;
    if(TIMELOCAL)
    {
        key = tic();
    }

    printf("Attempting to populate matrix with %d elements\n", nnz);

    for(uint32_t i=0; i<nnz; i++)
    {
        col += randint(0, step);

        while(col >= colmax)
        {
            if(col < 0) { col = 0; break; }

            row += 1;
            col -= colmax;
        }

        if(row >= rowmax) { break; }

        if(i % updateDelay == 0)
        {
            printf("%.2f%% populated...\n", (double)i*100 / (double)nnz);
        }

        double value = (double)(rand())/100;
        appendElement(matrix, row, col, value);

        valuesSet++;
    }

    if(TIMELOCAL)
    {
        printf("Successfully populated %d/%d elements in ", valuesSet, nnz);
        toc("", key);
    }
    else
    {
        printf("Successfully populated %d/%d elements\n", valuesSet, nnz);
    }
}



// Function to print the raw properties of the matrix
void printRawMatrix(Sparse* matrix)
{
    printf("  Unformatted CSR matrix\n");
    printf("Rows: %d\n", matrix->rows);
    printf("Cols: %d\n", matrix->cols);
    printf("NNZ:  %d\n", matrix->nnz);

    int rowlim = matrix->rows;
    int nnzlim = matrix->nnz;
    int i;

    printf("Row Pointers: ");
    for (i=0; i<rowlim; i++) {
        printf("%d ", matrix->row_ptr[i]);
    }
    printf("\n");


    printf("Column Indices: ");
    for (i=0; i<nnzlim; i++) {
        printf("%d ", matrix->col_idx[i]);
    }
    printf("\n");


    printf("Values: ");
    for (i=0; i<nnzlim; i++) {
        printf("%.2lf ", matrix->values[i]);
    }
    printf("\n");
}



// Iterates matrix elements and prints in ascending column format
void printMatrix(Sparse* matrix)
{
    printf("  Formatted CSR matrix\n");

    for(uint32_t i=0; i<matrix->nnz; i++)
    {
        uint32_t row = matrix->rows;
        for(uint32_t j=0; j<matrix->rows; j++)
        {
            if(matrix->row_ptr[j] > i)
            {
                row = j - 1;
                break;
            }
            else if(matrix->row_ptr[j] == i)
            {
                row = j;
                break;
            }
        }

        // Format less one space if negative
        if(matrix->values[i] < 0)
        {
            printf("(%d, %d)  %.4f\n", row, matrix->col_idx[i], matrix->values[i]);
        }
        else
        {
            printf("(%d, %d)   %.4f\n", row, matrix->col_idx[i], matrix->values[i]);
        }
    }
}




void csrMatrixVectorMultiply(Sparse* matrix, double* vector, double* result)
{
    int key;
    if(TIMELOCAL)
    {
        key = tic();
    }

    double row_result;
    register uint32_t i, j;

    for(i=0; i<matrix->rows - 1; i++)
    {
        row_result = 0.0f;

        // iterate row elements using col indices stored in row_ptr
        for(j=matrix->row_ptr[i]; j<matrix->row_ptr[i + 1]; j++)
        {
            row_result += matrix->values[j]*vector[matrix->col_idx[j]];
        }

        result[i] = row_result;
    }

    // iterate last row of the matrix
    row_result = 0.0;
    
    for(j=matrix->row_ptr[matrix->rows - 1]; j<matrix->nnz; j++)
    {
        row_result += matrix->values[j]*vector[matrix->col_idx[j]];
    }

    result[matrix->rows - 1] = row_result;

    if(TIMELOCAL)
    {
        toc("Multiplication computed in", key);
    }
}


void populateVector(double* vector, uint32_t size)
{
    for(uint32_t i=0; i<size; i++)
    {
        vector[i] = (double)randint(-1000, 1000);
    }
}

