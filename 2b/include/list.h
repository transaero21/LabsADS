#ifndef LIST_H
#define LIST_H

typedef struct Node {
    void *value;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    int length;
} Queue;

int pushList(Queue *queue, void *obj);
void *popList(Queue *queue);
void *getList(Queue *queue);

#endif //LIST_H
