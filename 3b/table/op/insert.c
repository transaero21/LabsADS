#include <string.h>

#include "../table.h"

static void moveKeySpaces(Table *table, int i);
static int setupNewKeySpace(Table *table, int i, int ptr, const char *key);
static void setupNewNode(Table *table, const char *info, int ptr, int nPtr, int release);
static int getReleaseInfo(Table *table, KSData ksData);

int insert(Table *table, const char *key, const char *info) {
    int i = searchForIndex(table, 0, table->tData.csize - 1, key, 1);
    if (i >= table->tData.msize) return 0;

    KSData ksData = getKSData(table, i);
    int cmp = ksData.ptr ? compareKeys(table, i, key) != 0 : 1;
    if (cmp + table->tData.csize >= table->tData.msize) return 0;

    int ptr = getEndOfFile(table), pPtr = cmp ? 0 : ksData.ptr;
    if (cmp) moveKeySpaces(table, i);
    int release = !cmp ? getReleaseInfo(table, ksData) : 1;

    if (cmp) ptr = setupNewKeySpace(table, i, ptr, key);
    else {
        ksData.ptr = ptr;
        setKSData(table, ksData, i);
    }
    setupNewNode(table, info, ptr, pPtr, release);

    return 1;
}

static void moveKeySpaces(Table *table, int i) {
    for (int j = table->tData.csize++; j > i; j--) {
        setKSData(table, getKSData(table, j - 1), j);
    }
    setTableParams(table, 0);
}


static int setupNewKeySpace(Table *table, int i, int ptr, const char *key) {
    KSData ksData = {.start = ptr, .length = (int) strlen(key)};
    ksData.ptr = ksData.start + ksData.length;
    setKSData(table, ksData, i);
    fseek(table->file, ksData.start, SEEK_SET);
    fwrite(key, sizeof(char), ksData.length, table->file);
    return ksData.ptr;
}

static void setupNewNode(Table *table, const char *info, int ptr, int nPtr, int release) {
    NData nData = {.ptr = nPtr, .length = (int) strlen(info), .release = release};
    fseek(table->file, ptr, SEEK_SET);
    fwrite(&nData, sizeof(NData), 1, table->file);
    fwrite(info, sizeof(char), nData.length, table->file);
}

static int getReleaseInfo(Table *table, KSData ksData) {
    NData nData;
    fseek(table->file, ksData.ptr, SEEK_SET);
    fread(&nData, sizeof(NData), 1, table->file);
    return nData.release + 1;
}