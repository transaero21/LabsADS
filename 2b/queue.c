#include "include/queue.h"

int push(Queue *queue, void *obj) {
#ifdef USE_LIST
    return pushList(queue, obj);
#else
    return pushArray(queue, obj);
#endif
}

void *pop(Queue *queue) {
#ifdef USE_LIST
    return popList(queue);
#else
    return popArray(queue);
#endif
}

void *get(Queue *queue) {
#ifdef USE_LIST
    return getList(queue);
#else
    return getArray(queue);
#endif
}
