#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

Table *initTable() {
    Table *table = NULL;
    int size;

    printf("Enter maximum table size: ");
    if (!readInt(&size, isPositive)) return NULL;
    table = calloc(1, sizeof(Table));
    table->ks = calloc(size, sizeof(KeySpace));
    table->msize = size;

    return table;
}

void destroyTable(Table *table) {
    for (int i = 0; i < table->msize; i++) {
        destroyNodes(table->ks[i].head);
    }
    free(table->ks), free(table);
}

static void destroyNodes(Node *node) {
    while (node) {
        Node *next = node->next;
        free(node->info), free(node);
        node = next;
    }
}

Table *temporaryTable(Table *table) {
    Table *lTable = calloc(1, sizeof(Table));
    lTable->ks = calloc(table->msize, sizeof(KeySpace));
    lTable->msize = table->msize;
    return lTable;
}

void insert(Table *table, int key, const char *info) {
    int i = getHash(key) % table->msize, release = 1;
    Node *new = calloc(1, sizeof(Node));

    new->key = key;
    new->next = table->ks[i].head;
    new->info = strdup(info);
    for (Node *node = new->next; node; node = node->next) {
        if (node->key == key) {
            release = node->release + 1;
            break;
        }
    }
    new->release = release;
    table->ks[i].head = new;
}

int delete(Table *table, int key, int release) {
    int i = getHash(key) % table->msize, rm = 0;
    Node *node = table->ks[i].head, *prev = NULL;

    while(node) {
        if (node->key == key && (!release || release == node->release)) {
            rm++;
            if (prev) prev->next = node->next;
            else table->ks[i].head = node->next;
            free(node->info), free(node);
            if (release) break;
            if (!prev) node = table->ks[i].head;
            else node = prev->next;
        } else {
            prev = node, node = node->next;
        }
    }

    return rm;
}

void search(Table *table, int key, int release, void (*println)(int, const char *, int)) {
    int i = getHash(key) % table->msize;
    Node *node = table->ks[i].head;

    while (node) {
        if (node->key == key && (!release || node->release == release)) break;
        node = node->next;
    }

    Table *lTable = temporaryTable(table);
    if (node) {
        for (Node *pNode = NULL; node; node = node->next) {
            if (node->key == key) {
                Node *iNode = calloc(1, sizeof(Node));
                iNode->release = node->release;
                iNode->key = node->key;
                iNode->info = strdup(node->info);
                if (!pNode) lTable->ks[i].head = iNode;
                else pNode->next = iNode;
                pNode = iNode;
            }
            if (release) break;
        }
    }

    print(lTable, println);
    destroyTable(lTable);
}

void import(Table *table, int *keys, char **info, int size) {
    for (int i = 0; i < table->msize; i++) {
        destroyNodes(table->ks[i].head);
        table->ks[i].head = NULL;
    }
    for (int i = 0; i < size; i++) {
        insert(table, keys[i], info[i]);
    }
}

int clean(Table *table) {
    int rm = 0;

    for (int i = 0; i < table->msize; i++) {
        Node *node = table->ks[i].head;
        for (; node; node = node->next) {
            Node *next = node->next, *prev = node;
            while (next) {
                if (next->key == node->key) {
                    prev->next = next->next;
                    free(next->info), free(next);
                    next = prev->next;
                    rm++;
                } else {
                    prev = next;
                    next = next->next;
                }
            }
        }
    }

    return rm;
}

void print(Table *table, void (*println)(int, const char *, int)) {
    for (int i = 0; i < table->msize; i++) {
        Node *node = table->ks[i].head;
        for (int j = 0; node; node = node->next, j++) {
            println(node->key, node->info, node->release);
        }
    }
}

static int getHash(int key) {
    int hash = 0;
    for (; key > 1; key /= 10) {
        hash = hash * 31 + key % 10;
    }
    return hash;
}