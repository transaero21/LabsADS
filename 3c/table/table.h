#ifndef TABLE_H
#define TABLE_H

#include "../include/utils.h"

typedef struct Node {
    int key;
    int release;
    char *info;
    struct Node *next;
} Node;

typedef struct KeySpace {
    Node *head;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    int msize;
} Table;

Table *initTable();
void destroyTable(Table *table);

static void destroyNodes(Node *node);

void insert(Table *table, int key, const char *info);
int delete(Table *table, int key, int release);
void search(Table *table, int key, int release, void (*println)(int, const char *, int));
void import(Table *table, int *keys, char **info, int size);
int clean(Table *table);
void print(Table *table, void (*println)(int, const char *, int));

static int getHash(int key);

#endif //TABLE_H
