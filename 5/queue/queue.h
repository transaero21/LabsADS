#ifndef QUEUE_H
#define QUEUE_H

typedef struct QNode {
    void *value;
    struct QNode *next;
} QNode;

typedef struct QList {
    QNode *head;
} Queue;

int push(Queue *queue, void *obj);
void *pop(Queue *queue);
int isEmpty(Queue *queue);

#endif //QUEUE_H
