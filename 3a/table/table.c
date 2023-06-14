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
    for (int i = 0; i < table->csize; i++) {
        while (table->ks[i].node) {
            destroyNode(&table->ks[i], table->ks[i].node);
        }
        free(table->ks[i].key);
    }
    free(table->ks);
    free(table);
}

Node *initNode(Node *next, int release, const char *value) {
    Node *node = calloc(1, sizeof(Node));
    node->release = release;
    node->info = strdup(value);
    node->next = next;
    return node;
}

void destroyNode(KeySpace *ks, Node *node) {
    free(node->info);
    if (ks->node != node) {
        Node *prev = ks->node;
        for (; prev->next != node; prev = prev->next);
        prev->next = node->next;
    } else ks->node = ks->node->next;
    free(node);
}

void deleteByIndex(Table *table, int i, int nodes) {
    KeySpace *ks = &table->ks[i];
    if (ks->key) {
        if (nodes) while(ks->node) destroyNode(ks, ks->node);
        free(ks->key);
        for (int j = i; j + 1 < table->csize; j++) {
            table->ks[j] = table->ks[j + 1];
        }
        table->ks[--table->csize].key = NULL;
    }
}

Node *getNodeByParam(Table *table, const char *key, int release, int *index) {
    *index = searchForIndex(table, 0, table->csize - 1, key, 0);
    if (*index >= 0) {
        Node *node = table->ks[*index].node;
        if (!release) return node;
        for (; node; node = node->next) {
            if (node->release == release) return node;
        }
    }
    return NULL;
}

int searchForIndex(Table *table, int from, int to, const char *key, int approx) {
    if (to >= from) {
        int mid = from + (to - from) / 2, cmp = compareKeys(table, mid , key);
        if (cmp > 0) return searchForIndex(table, from, mid - 1, key, approx);
        if (cmp < 0) return searchForIndex(table, mid + 1, to, key, approx);
        return mid;
    }
    return approx ? from : -1;
}

int compareKeys(Table *table, int i, const char *key) {
    if (!table->ks[i].key) return 1;
    return strcmp(table->ks[i].key, key);
}