#include <stdio.h>
#include <stdlib.h>

#include "include/matrix.h"
#include "include/utils.h"

int readMatrix(Matrix *mx) {
    printf("Enter the number of matrix lines (lines > 0): ");
    if (!getInt(&mx->size, isPositive)) return 0;

    for (int i = 0; i < mx->size; i++) {
        mx->lines = realloc(mx->lines, (i + 1) * sizeof(Line));
        mx->lines[i].values = NULL;
        if (!readMatrixLine(&mx->lines[i], i)) {
            destroyLines(mx->lines, i + 1);
            return 0;
        }
    }

    return 1;
}

int readMatrixLine(Line *line, int i) {
    printf("Enter size of [%d] (size > 0): ", i);
    if (!getInt(&line->size, isPositive)) return 0;

    line->values = malloc(line->size * sizeof(int));
    for (int j = 0; j < line->size; j++) {
        printf("Enter [%d][%d] value: ", i, j);
        if (!getInt(&line->values[j], NULL)) return 0;
    }

    pressEnterToContinue();
    clearOutput();

    return 1;
}

void printMatrix(Matrix mx) {
    printf("Your input matrix:\n");
    for (int i = 0; i < mx.size; i++) {
        for (int j = 0; j < mx.lines[i].size; j++) {
            if (j) printf("\t");
            printf("%d", mx.lines[i].values[j]);
        }
        printf("\n");
    }
}

void parseMatrix(Matrix mx) {
    int b[mx.size];

    for (int i = 0; i < mx.size; i++) {
        b[i] = getMaxRepeatOfLine(mx.lines[i]);
    }

    printf("Result: {");
    for (int i = 0; i < mx.size; i++) {
        if (i) printf(", ");
        printf("%d", b[i]);
    }
    printf("}\n");
}

int getMaxRepeatOfLine(Line ln) {
    int ret = 0;

    for (int i = 0; i < ln.size; i++) {
        int count = 0;
        for (int j = 0; j < ln.size; ++j) {
            if (ln.values[i] == ln.values[j]) count++;
        }
        if (count > ret) ret = count;
    }

    return ret;
}

void destroyLines(Line *lines, int i) {
    for (int j = 0; j < i; j++) {
        free(lines[j].values);
    }
    free(lines);
}

void destroyMatrix(Matrix *mx) {
    destroyLines(mx->lines, mx->size);
}