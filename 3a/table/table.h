#ifndef TABLE_H
#define TABLE_H

#include "../include/utils.h"

typedef struct Node {
    int release;
    char *info;
    struct Node *next;
} Node;

typedef struct KeySpace {
    char *key;
    Node *node;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    int csize;
    int msize;
} Table;

Table *initTable();
void destroyTable(Table *table);

int insert(Table *table, const char *key, const char *info);
int delete(Table *table, const char *key, int release);
void search(Table *table, const char *key, int release, void (*println)(const char *, const char *, int));
int import(Table *table, char **keys, char **info, int size);
void print(Table *table, void (*println)(const char *, const char *, int));

Node *initNode(Node *next, int release, const char *value);

void destroyNode(KeySpace *ks, Node *node);
Node *getNodeByParam(Table *table, const char *key, int release, int *index);
void deleteByIndex(Table *table, int i, int nodes);
int searchForIndex(Table *table, int from, int to, const char *key, int approx);
int compareKeys(Table *table, int i, const char *key);

#endif //TABLE_H
