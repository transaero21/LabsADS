#ifndef TABLE_H
#define TABLE_H

#include <string.h>

#include "../include/utils.h"

typedef struct NData {
    int release;
    int length;
    int ptr;
} NData;

typedef struct KSData {
    int start;
    int length;
    int ptr;
} KSData;

typedef struct TData {
    int csize;
    int msize;
    int tmp;
} TData;

typedef struct Table {
    TData tData;
    FILE *file;
    char *filename;
} Table;

#define MAGIC "CTABLE"
#define MAGIC_SIZE strlen(MAGIC)
#define PARAMS_OFFSET (MAGIC_SIZE + sizeof(TData))

Table *initTable();
void destroyTable(Table *table);
Table *temporaryTable(Table *table, int msize);
void optimizeTable(Table *table);

void setTableParams(Table *table, int init);
void getTableParams(Table *table);

int insert(Table *table, const char *key, const char *info);
int delete(Table *table, const char *key, int release);
void search(Table *table, const char *key, int release, void (*println)(const char *, const char *, int));
int import(Table *table, char **keys, char **info, int size);
void print(Table *table, void (*println)(const char *, const char *, int));

KSData getKSData(Table *table, int i);
void setKSData(Table *table, KSData ksData, int i);

int lookUpNode(Table *table, KSData ksData, int release, NData *nData, NData *pNData);
void copyNodes(Table *oTable, Table *nTable, int release, int ptr);

char *getKey(Table *table, int i);
int compareKeys(Table *table, int i, const char *key);

int searchForIndex(Table *table, int from, int to, const char *key, int approx);
int getFixatedOffset(Table *table);
int getEndOfFile(Table *table);

int verifyMagic(FILE *file);

#endif //TABLE_H
