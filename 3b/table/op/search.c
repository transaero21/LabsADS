
#include "../table.h"

static int setPrimaryKeySpace(Table *table, const char *key, int length);

void search(Table *table, const char *key, int release, void (*println)(const char *, const char *, int)) {
    int i = searchForIndex(table, 0, table->tData.csize - 1, key, 0);
    if (i < 0) return;

    KSData ksData = getKSData(table, i);
    NData nData, pNData = {.ptr = 0};
    if (!lookUpNode(table, ksData, release, &nData, &pNData)) return;

    Table *lTable = temporaryTable(table, 1);
    if (!lTable) return;
    lTable->tData.csize++;
    setTableParams(lTable, 0);

    int ptr = setPrimaryKeySpace(lTable, key, ksData.length);
    fseek(table->file, pNData.ptr ? pNData.ptr : ksData.ptr, SEEK_SET);
    copyNodes(table, lTable, release, ptr);

    print(lTable, println);
    destroyTable(lTable);
}

static int setPrimaryKeySpace(Table *table, const char *key, int length) {
    KSData lKSData = {.length = length, .start = getFixatedOffset(table)};
    lKSData.ptr = lKSData.length + lKSData.start;
    setKSData(table, lKSData, 0);
    fwrite(key, sizeof(char), lKSData.length, table->file);
    return lKSData.ptr;
}
