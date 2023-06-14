#include <stdio.h>

#include "include/array.h"

int pushArray(Queue *queue, void *obj) {
    if (queue->length != MAX_SIZE) {
        queue->length++;
        if (++queue->end > MAX_SIZE) queue->end = 1;
        queue->ptr[queue->end - 1] = obj;
    } else {
        printf("Buffer overflow, ");
        return 0;
    }

    return 1;
}

void *popArray(Queue *queue) {
    void *ret = NULL;

    if (queue->length) {
        queue->length--;
        if (++queue->front >= MAX_SIZE) queue->front = 0;
        ret = queue->ptr[(queue->front ? queue->front : MAX_SIZE) - 1];
    }

    return ret;
}

void *getArray(Queue *queue) {
    return queue->ptr[queue->front];
}