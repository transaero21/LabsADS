#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define MAX_BUF 64

enum FStatus {
    FAILED = -1,
    OPENED = 0,
    CREATED = 1
};

enum FRequest {
    OPEN = 0,
    CREATE = 1
};

char *readLine(FILE *file);
int readInt(int *i, int(*verify)(int));
enum FStatus getFile(FILE **file, char **filename, enum FRequest request);
int isPositive(int i);
void pressEnterToContinue();

#endif //UTILS_H
