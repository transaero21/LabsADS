#include <stdlib.h>

#include "table.h"

Table *initTable() {
    int msize = 0, ret = 1, fRet;
    Table *table = NULL;
    FILE *file = NULL;
    char *filename = NULL;

    if ((fRet = getFile(&file, &filename, CREATE)) == FAILED) return NULL;
    if (fRet == OPENED) {
        if (!verifyMagic(file)) {
            fprintf(stderr, "File corrupted or wrong format!\n");
            ret = 0;
        }
        fread(&msize, sizeof(int), 1, file);
    } else {
        printf("Enter maximum table size: ");
        ret = readInt(&msize, isPositive);
    }

    if (ret) {
        table = calloc(1, sizeof(Table));
        table->file = file;
        table->filename = filename;
        table->msize = msize;
        if (fRet == CREATED) {
            fwrite(&MAGIC, sizeof(char), MAGIC_SIZE, table->file);
            fwrite(&table->msize, sizeof(int), 1, table->file);
        }
    } else {
        free(filename);
        fclose(file);
    }

    return table;
}

void destroyTable(Table *table) {
    if (!table->temporary) optimizeTable(table);
    fclose(table->file);
    free(table->filename), free(table);
}


void insert(Table *table, int key, const char *info) {
    int i = getHash(key) % table->msize, eof = getEndOfFile(table);

    NData nData = {.next = getStart(table, i)};
    NData nNData = {.key = key, .release = 1, .next = nData.next, .length = (int) strlen(info)};
    while (nData.next) {
        nData = getNext(table, nData.next);
        if (nData.key == key) {
            nNData.release += nData.release;
            break;
        }
    }

    setStart(table, i, eof);
    fseek(table->file, eof, SEEK_SET);
    fwrite(&nNData, sizeof(NData), 1, table->file);
    fwrite(info, sizeof(char), nNData.length, table->file);
}

int delete(Table *table, int key, int release) {
    int i = getHash(key) % table->msize, rm = 0;

    int pPtr = 0;
    NData nData = {.next = getStart(table, i)}, pNData = {.next = 0};
    while(nData.next) {
        nData = getNext(table, nData.next);
        if (nData.key == key && (!release || release == nData.release)) {
            rm++;
            if (pNData.next) {
                pNData.next = nData.next;
                fseek(table->file, pPtr, SEEK_SET);
                fwrite(&pNData, 1, sizeof(pNData), table->file);
            } else setStart(table, i, nData.next);
            if (release) break;
        } else {
            pNData = nData;
            pPtr = (int) (ftell(table->file) - sizeof(NData));
        }
    }

    return rm;
}

void search(Table *table, int key, int release, void (*println)(int, const char *, int)) {
    int i = getHash(key) % table->msize;
    Table *lTable = temporaryTable(table);

    NData nData = {.next = getStart(table, i)}, pNData = {.next = 0};
    while(nData.next) {
        nData = getNext(table, nData.next);
        if (nData.key == key && (!release || release == nData.release)) {
            char *info = calloc(nData.length + 1, sizeof(char));
            fread(info, nData.length, sizeof(char), table->file);
            int eof = getEndOfFile(lTable);
            NData nNData = {.next = 0, .release = nData.release, .length = nData.length, .key = nData.key};
            if (pNData.next) {
                fseek(lTable->file, -(pNData.length + (int) sizeof(NData)), SEEK_CUR);
                fwrite(&pNData, sizeof(NData), 1, lTable->file);
            } else setStart(lTable, i, eof);
            fseek(lTable->file, eof, SEEK_SET);
            fwrite(&nNData, sizeof(NData), 1, lTable->file);
            fwrite(info, sizeof(char), nNData.length, lTable->file);
            free(info);
            if (release) break;
            pNData = nNData;
            pNData.next = (int) ftell(lTable->file);
        }
    }

    print(lTable, println);
    remove(lTable->filename);
    destroyTable(lTable);
}

int clean(Table *table) {
    int rm = 0, start;

    for (int i = 0; i < table->msize; i++) {
        if (!(start = getStart(table, i))) continue;
        for (NData nData = getNext(table, start); nData.next;) {
            int oPtr = (int) (ftell(table->file) - sizeof(NData)), pPtr = oPtr;
            NData nNData = getNext(table, nData.next), pNData = nData;
            do {
                if (nNData.key == nData.key) {
                    rm++;
                    pNData.next = nNData.next;
                    fseek(table->file, pPtr, SEEK_SET);
                    fwrite(&pNData, 1, sizeof(pNData), table->file);
                } else {
                    pNData = nNData;
                    pPtr = (int) (ftell(table->file) - sizeof(NData));
                }
                nNData = getNext(table, nNData.next);
            } while (nNData.release);
            nData = getNext(table, oPtr);
            if (nData.next) nData = getNext(table, nData.next);
        }
    }

    return rm;
}

void import(Table *table, int *keys, char **info, int size) {
    Table *lTable = temporaryTable(table);

    for (int i = 0; i < size; i++) {
        insert(lTable, keys[i], info[i]);
    }

    fflush(lTable->file);
    rename(lTable->filename, table->filename);
    table->file = freopen(table->filename, "r+b", table->file);
    destroyTable(lTable);
}

void print(Table *table, void (*println)(int, const char *, int)) {
    for (int i = 0; i < table->msize; i++) {
        NData nData = {.next = getStart(table, i)};
        while(nData.next) {
            nData = getNext(table, nData.next);
            char *info = calloc(nData.length + 1, sizeof(char));
            fread(info, nData.length, sizeof(char), table->file);
            println(nData.key, info, nData.release);
            free(info);
        }
    }
}

static Table *temporaryTable(Table *table) {
    Table *lTable = calloc(1, sizeof(Table));
    lTable->filename = calloc(strlen(table->filename) + 5, sizeof(char));
    strcat(lTable->filename, table->filename);
    strcat(lTable->filename, ".tmp");
    lTable->file = fopen(lTable->filename, "w+b");
    lTable->msize = table->msize;
    lTable->temporary = 1;
    fwrite(&MAGIC, sizeof(char), MAGIC_SIZE, lTable->file);
    fwrite(&table->msize, sizeof(int), 1, lTable->file);
    return lTable;
}

static void optimizeTable(Table *table) {
    Table *lTable = temporaryTable(table);
    for (int i = 0; i < table->msize; i++) {
        NData nData = {.next = getStart(table, i)}, pNData = {.next = 0};
        while(nData.next) {
            nData = getNext(table, nData.next);
            char *info = calloc(nData.length + 1, sizeof(char));
            fread(info, nData.length, sizeof(char), table->file);
            int eof = getEndOfFile(lTable);
            NData nNData = {.next = 0, .release = nData.release, .length = nData.length, .key = nData.key};
            if (pNData.next) {
                fseek(lTable->file, -(pNData.length + (int) sizeof(NData)), SEEK_CUR);
                fwrite(&pNData, sizeof(NData), 1, lTable->file);
            } else setStart(lTable, i, eof);
            fseek(lTable->file, eof, SEEK_SET);
            fwrite(&nNData, sizeof(NData), 1, lTable->file);
            fwrite(info, sizeof(char), nNData.length, lTable->file);
            free(info);
            pNData = nNData;
            pNData.next = (int) ftell(lTable->file);
        }
    }
    rename(lTable->filename, table->filename);
    destroyTable(lTable);
}

static int verifyMagic(FILE *file) {
    char *real = calloc(MAGIC_SIZE + 1, sizeof(char));
    fseek(file, 0, SEEK_SET);
    fread(real, sizeof(char), MAGIC_SIZE, file);
    int ret = !strcmp(real, MAGIC);
    free(real);
    return ret;
}

static NData getNext(Table *table, int next) {
    NData nData = {.key = 0, .release = 0, .length = 0, .next = 0};
    if (!next) return nData;
    fseek(table->file, next, SEEK_SET);
    fread(&nData, sizeof(NData), 1, table->file);
    return nData;
}

static void setStart(Table *table, int i, int start) {
    fseek(table->file, PARAMS_OFFSET + i * sizeof(int), SEEK_SET);
    fwrite(&start, sizeof(int), 1, table->file);
}

static int getStart(Table *table, int i) {
    int start = 0;
    fseek(table->file, PARAMS_OFFSET + i * sizeof(int), SEEK_SET);
    fread(&start, sizeof(int), 1, table->file);
    return start;
}

static int getEndOfFile(Table *table) {
    fseek(table->file, 0, SEEK_END);
    int rEnd = (int) ftell(table->file), fEnd = (int) (PARAMS_OFFSET + table->msize * sizeof(int));
    return fEnd > rEnd ? fEnd : rEnd;
}

static int getHash(int key) {
    int hash = 0;
    for (; key > 1; key /= 10) {
        hash = hash * 31 + key % 10;
    }
    return hash;
}
