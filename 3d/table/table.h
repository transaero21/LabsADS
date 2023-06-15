#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <string.h>

#include "../include/utils.h"

typedef struct NData {
    int key;
    int release;
    int length;
    int next;
} NData;

typedef struct Table {
    FILE *file;
    char *filename;
    int msize;
    int temporary;
} Table;

#define MAGIC "CTABLE"
#define MAGIC_SIZE strlen(MAGIC)
#define PARAMS_OFFSET (MAGIC_SIZE + sizeof(int))

Table *initTable();
void destroyTable();

void insert(Table *table, int key, const char *info);
int delete(Table *table, int key, int release);
void search(Table *table, int key, int release, void (*println)(int, const char *, int));
void import(Table *table, int *keys, char **info, int size);
int clean(Table *table);
void print(Table *table, void (*println)(int, const char *, int));

static Table *temporaryTable(Table *table);
static void optimizeTable(Table *table);
static int verifyMagic(FILE *file);

static NData getNext(Table *table, int next);
static void setStart(Table *table, int i, int start);
static int getStart(Table *table, int i);
static int getEndOfFile(Table *table);

static int getHash(int key);

#endif //TABLE_H
