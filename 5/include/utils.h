#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define MAX_BUF 64

char *readLine(FILE *file);
int readInt(int *i, int(*verify)(int));
int getFile(FILE **file, const char *mode);
void toLower(char *string);
int stringToInt(const char* string, int *i);
void pressEnterToContinue();

#endif //UTILS_H
