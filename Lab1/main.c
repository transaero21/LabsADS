#include <stdio.h>
#include <stdlib.h>

#include "include/matrix.h"
#include "include/utils.h"

int main() {
    Matrix *mx = calloc(1, sizeof(Matrix));

    clearOutput();
    if (readMatrix(mx)) {
        clearOutput();
        printMatrix(*mx);
        parseMatrix(*mx);
        destroyMatrix(mx);
    } else {
        printf("\nExiting!\n");
    }
    free(mx);

    return 0;
}
