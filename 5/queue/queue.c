#include <stdlib.h>

#include "queue.h"

int push(Queue *queue, void *obj) {
    QNode *new = calloc(1, sizeof(QNode));
    new->value = obj;

    if (queue->head) {
        QNode *node = queue->head;
        while (node->next) {
            node = node->next;
        }
        node->next = new;
    } else queue->head = new;

    return 1;
}

void *pop(Queue *queue) {
    void *ret = NULL;

    if (queue->head) {
        ret = queue->head->value;
        QNode *node = queue->head;
        queue->head = node->next;
        free(node);
    }

    return ret;
}

int isEmpty(Queue *queue) {
    return queue->head == NULL;
}
