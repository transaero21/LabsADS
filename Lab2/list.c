#include <stdlib.h>

#include "include/list.h"

int pushList(Queue *queue, void *obj) {
    Node *new = calloc(1, sizeof(Node));
    new->value = obj;

    queue->length++;
    if (queue->head) {
        Node *node = queue->head;
        while (node->next) {
            node = node->next;
        }
        node->next = new;
    } else queue->head = new;

    return 1;
}

void *popList(Queue *queue) {
    void *ret = NULL;

    if (queue->head) {
        queue->length--;
        ret = queue->head->value;
        Node *node = queue->head;
        queue->head = node->next;
        free(node);
    }

    return ret;
}

void *getList(Queue *queue) {
    return queue->head->value;
}