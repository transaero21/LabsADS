#ifndef QUEUE_H
#define QUEUE_H

#ifdef USE_LIST
#include "list.h"
#else
#include "array.h"
#endif

int push(Queue *queue, void *pax);
void *pop(Queue *queue);
void *get(Queue *queue);

#endif //QUEUE_H
