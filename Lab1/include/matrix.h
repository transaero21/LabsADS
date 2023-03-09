#ifndef MATRIX_H
#define MATRIX_H

typedef struct Line {
    int *values;
    int size;
} Line;

typedef struct Matrix {
    Line *lines;
    int size;
} Matrix;

int readMatrix(Matrix *mx);
int readMatrixLine(Line *line, int i);
void printMatrix(Matrix mx);
void parseMatrix(Matrix mx);
int getMaxRepeatOfLine(Line ln);
void destroyLines(Line *lines, int i);
void destroyMatrix(Matrix *mx);

#endif //MATRIX_H
