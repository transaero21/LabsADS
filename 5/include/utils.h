#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define MAX_BUF 64

char *readLine(FILE *file);
int readInt(int *i, int(*verify)(int));
void pressEnterToContinue();

#endif //UTILS_H
