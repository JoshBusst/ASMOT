#include <time.h>
#include <inttypes.h>
#include <stdio.h>

#include "lib.h"
#include "sparse.h"

#define SIZE 1000

double result[SIZE];
double operand[SIZE];
Sparse* matrix;

void veccpy(double[], const double[], uint32_t);
void printvec(double[], uint32_t);

void veccpy(double destination[], const double source[], uint32_t size)
{
    for (uint32_t i=0; i<size; i++)
    {
        destination[i] = source[i];
    }
}

void printvec(double vector[], uint32_t size)
{
    for(uint32_t i=0; i<size; i++)
    {
        printf(" %.2f", (float)vector[i]);
    }

    printf("\n");
}



int main()
{
    libInit();

    uint32_t rows = SIZE;
    uint32_t cols = SIZE;
    double d = 0.00001;

    // benchmarking paramters
    // double times[100];
    int n = 100;
    int key, i;


    printf("Successfully allocated %.6lf Gb of memory to populate matrix.\n", (float)(memory/1e9));
    toc("Matrix was generated in", key);

    matrix = createSparseMatrix(rows, cols, d);
    populateMatrix(matrix);

    uint32_t row, col;
    double rand;
    printf("Running\n");
    for(int i=0; i<n; i++)
    {
        row = randint(0,rows-1);
        col = randint(0,cols-1);
        rand = randint(-1000, 1000)/10.0;

        // key = tic();
        appendElement(matrix, row, col, rand);
        // times[i] = toc("NULL",key);
    }
    // printf("Average insertion time %.2fms", avg(times, n));
    
    // populateVector(operand, SIZE);

    // for(i=0; i<n; i++)
    // {
    //     key = tic();
    //     csrMatrixVectorMultiply(matrix, operand, result);
    //     times[i] = toc("NULL",key);

    //     populateVector(operand, SIZE);
    // }
    // printf("Average product compute time %.3fms\n\n", avg(times, n));


    // Free memory
    freeMatrix(matrix);

    return 0;
}