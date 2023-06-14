#ifndef ARRAY_H
#define ARRAY_H

#define MAX_SIZE 128

typedef struct Array {
    void *ptr[MAX_SIZE];
    int front;
    int end;
    int length;
} Queue;

int pushArray(Queue *queue, void *obj);
void *popArray(Queue *queue);
void *getArray(Queue *queue);

#endif //ARRAY_H
