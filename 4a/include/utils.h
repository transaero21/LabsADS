#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include <time.h>

#define MAX_BUF 64

char *readLine(FILE *file);
int readInt(int *i, int(*verify)(int));
int getFile(FILE **file);
int stringToInt(const char* string, int *i);
void pressEnterToContinue(clock_t time);

#endif //UTILS_H
