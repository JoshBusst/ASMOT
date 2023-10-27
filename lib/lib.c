#include "lib.h"

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>



// Generates a random integer in the range [min, max]
int32_t randint(int32_t min, int32_t max)
{
    assert(min < max);

    int32_t random = (rand() % (max - min + 1)) + min;

    return random;
}



// Prints a common, formatted heading to the console
void printHeading(char str[])
{
    printf("<- ### %s ### ->", str);
}



// Functions to time software. Calls are not unique or protected. Must only be used sparingly.
#define MAX_KEYS 1000
static clock_t clocks[MAX_KEYS];
static int numKeys;

int tic(void)
{
    if(numKeys++ > MAX_KEYS) { numKeys = 0; }

    clocks[numKeys] = clock();

    return numKeys;
}

double toc(char str[], int clockID)
{
    clock_t now = clock();
    double elapsed_time = (double)(now - clocks[clockID]) / CLOCKS_PER_SEC*1000.0;
    
    if(strcmp(str, "NULL"))
    {
        printf("%s - %.1fms\n", str, elapsed_time);
    }

    return elapsed_time;
}



// Inserts a uint32_t into a list
void insertint32(uint32_t arr[], uint32_t size, uint32_t index, uint32_t value)
{
    // Check if the index is within bounds
    if (index < 0 || index > size) {
        printf("Invalid index for insertion.\n");
        return;
    }

    // Shift elements to the right to create space for the new value
    for (int i=size; i>index; i--)
    {
        arr[i] = arr[i - 1];
    }

    // Insert the new value at the specified index
    arr[index] = value;
}



// Inserts a double into a list
void insertdouble(double arr[], uint32_t size, uint32_t index, double value)
{
    // Check if the index is within bounds
    if (index < 0 || index > size) {
        printf("Invalid index for insertion.\n");
        return;
    }

    // Shift elements to the right to create space for the new value
    for (int i=size; i>index; i--)
    {
        arr[i] = arr[i - 1];
    }

    // Insert the new value at the specified index
    arr[index] = value;
}



// Initialises the module
void libInit(void)
{
    // For random lib
    srand(time(NULL));

    // For tic toc
    numKeys = 0;
}



// Check if input x is bounded by min and max
int isbounded(float x, float min, float max)
{
    return (int)(x < max && x > min);
}



// take the average of an array
double avg(double arr[], uint32_t size)
{
    double value = 0.0f;

    for(uint32_t i=0; i<size; i++)
    {
        value += arr[i];
    }

    value /= size;
    return value;
}