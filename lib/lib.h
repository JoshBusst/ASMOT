#ifndef LIB_H
#define LIB_H

#include <inttypes.h>


int32_t randint(int32_t, int32_t);
void printHeading(char[]);
void libInit(void);
int tic(void);
double toc(char[], int);
void insertint32(uint32_t[], uint32_t, uint32_t, uint32_t);
void insertdouble(double[], uint32_t, uint32_t, double);
int isbounded(float, float, float);
double avg(double[], uint32_t size);

#endif