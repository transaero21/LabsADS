#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
        if (fRet == CREATED) {
            table->tData.msize = msize;
            table->tData.tmp = 0;
            setTableParams(table, 1);
        } else getTableParams(table);
    } else {
        free(filename);
        fclose(file);
    }

    return table;
}

void destroyTable(Table *table) {
    if (!table->tData.tmp) optimizeTable(table);
    fclose(table->file);
    free(table->filename), free(table);
}

Table *temporaryTable(Table *table, int msize) {
    Table *lTable = calloc(1, sizeof(Table));
    lTable->filename = calloc(strlen(table->filename) + 5, sizeof(char));
    strcat(lTable->filename, table->filename);
    strcat(lTable->filename, ".tmp");
    lTable->file = fopen(lTable->filename, "w+b");
    lTable->tData.msize = msize;
    lTable->tData.tmp = 1;
    setTableParams(lTable, 1);
    return lTable;
}

void optimizeTable(Table *table) {
    Table *lTable = temporaryTable(table, table->tData.msize);
    for (int i = 0, offset = getFixatedOffset(table); i < table->tData.csize; i++) {
        KSData ksData = getKSData(table, i);
        char *key = getKey(table, i);
        fseek(table->file, ksData.ptr, SEEK_SET);
        ksData.start = offset;
        ksData.ptr = ksData.start + ksData.length;
        setKSData(lTable, ksData, i);
        fseek(lTable->file, ksData.start, SEEK_SET);
        fwrite(key, sizeof(char), ksData.length, lTable->file);
        copyNodes(table, lTable, 0, ksData.ptr);
        offset = (int) ftell(lTable->file);
        free(key);
    }
    lTable->tData.csize = table->tData.csize;
    setTableParams(lTable, 0);
    rename(lTable->filename, table->filename);
    destroyTable(lTable);
}

void setTableParams(Table *table, int init) {
    fseek(table->file, init ? 0 : MAGIC_SIZE, SEEK_SET);
    if (init) fwrite(&MAGIC, sizeof(char), MAGIC_SIZE, table->file);
    fwrite(&table->tData, sizeof(TData), 1, table->file);
}

void getTableParams(Table *table) {
    if (verifyMagic(table->file)) {
        fseek(table->file, MAGIC_SIZE, SEEK_SET);
        fread(&table->tData, sizeof(TData), 1, table->file);
    }
}

int searchForIndex(Table *table, int from, int to, const char *key, int approx) {
    if (to >= from) {
        int mid = from + (to - from) / 2, cmp = compareKeys(table, mid, key);
        if (cmp > 0) return searchForIndex(table, from, mid - 1, key, approx);
        if (cmp < 0) return searchForIndex(table, mid + 1, to, key, approx);
        return mid;
    }
    return approx ? from : -1;
}

int compareKeys(Table *table, int i, const char *key) {
    char *iKey = getKey(table, i);
    int ret = strcmp(iKey, key);
    free(iKey);
    return ret;
}

void setKSData(Table *table, KSData ksData, int i) {
    fseek(table->file, PARAMS_OFFSET + sizeof(KSData) * i, SEEK_SET);
    fwrite(&ksData, sizeof(KSData), 1, table->file);
}

KSData getKSData(Table *table, int i) {
    KSData ksData = {.length = 0, .ptr = 0, .start = 0 };
    fseek(table->file, PARAMS_OFFSET + sizeof(KSData) * i, SEEK_SET);
    fread(&ksData, sizeof(KSData), 1, table->file);
    return ksData;
}

int getEndOfFile(Table *table) {
    if (!table->tData.csize) return getFixatedOffset(table);
    else {
        fseek(table->file, 0, SEEK_END);
        return (int) ftell(table->file);
    }
}

int getFixatedOffset(Table *table) {
    return (int) (PARAMS_OFFSET + table->tData.msize * sizeof(KSData));
}

char *getKey(Table *table, int i) {
    KSData ksData = getKSData(table, i);
    char *key = calloc(ksData.length + 1, sizeof(char));
    fseek(table->file, ksData.start, SEEK_SET);
    fread(key, sizeof(char), ksData.length, table->file);
    return key;
}

int verifyMagic(FILE *file) {
    char *real = calloc(MAGIC_SIZE + 1, sizeof(char));
    fseek(file, 0, SEEK_SET);
    fread(real, sizeof(char), MAGIC_SIZE, file);
    int ret = !strcmp(real, MAGIC);
    free(real);
    return ret;
}

int lookUpNode(Table *table, KSData ksData, int release, NData *nData, NData *pNData) {
    int pPtr = 0, cnt;
    fseek(table->file, ksData.ptr, SEEK_SET);
    if (!release) return 1;
    do {
        fread(nData, sizeof(NData), 1, table->file);
        if ((cnt = nData->ptr && nData->release != release)) {
            pPtr = (int) (ftell(table->file) - sizeof(NData));
            fseek(table->file, nData->ptr, SEEK_SET);
            *pNData = *nData;
        } else {
            if (pPtr) fseek(table->file, pPtr, SEEK_SET);
            else fseek(table->file, -(int) sizeof(NData), SEEK_CUR);
        }
    } while (cnt);
    if (nData->release != release) return 0;
    return 1;
}

void copyNodes(Table *oTable, Table *nTable, int release, int ptr) {
    int old, cmp;
    NData nData;
    do {
        fread(&nData, sizeof(NData), 1, oTable->file);
        char *info = calloc(nData.length + 1, sizeof(char));
        fread(info, sizeof(char), nData.length, oTable->file);
        if (nData.ptr && !release) fseek(oTable->file, nData.ptr, SEEK_CUR);
        ptr += (int) sizeof(NData) + nData.length, old = nData.ptr, nData.ptr = ptr;
        fwrite(&nData, sizeof(NData), 1, nTable->file);
        fwrite(info, sizeof(char), nData.length, nTable->file);
        free(info);
        cmp = old && release != nData.release;
        if (cmp) fseek(oTable->file, old, SEEK_SET);
    } while (cmp);
    nData.ptr = 0;
    fseek(nTable->file, -(int) (sizeof(NData) + nData.length), SEEK_CUR);
    fwrite(&nData, sizeof(NData), 1, nTable->file);
    fseek(nTable->file, nData.length, SEEK_CUR);
}
