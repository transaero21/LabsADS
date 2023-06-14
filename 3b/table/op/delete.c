#include "../table.h"

static void destroyKeySpace(Table *table, int i);
static void updatePointers(Table *table, KSData *ksData, int release, NData nData, NData pNData);

int delete(Table *table, const char *key, int release) {
    int i = searchForIndex(table, 0, table->tData.csize - 1, key, 0);
    if (i < 0) return 0;

    KSData ksData = getKSData(table, i);
    NData nData, pNData = {.ptr = 0};
    if (!lookUpNode(table, ksData, release, &nData, &pNData)) return 0;

    updatePointers(table, &ksData, release, nData, pNData);
    setKSData(table, ksData, i);
    if (!ksData.ptr) destroyKeySpace(table, i);

    return 1;
}

static void destroyKeySpace(Table *table, int i) {
    for (int j = i + 1; j < table->tData.csize; j++) {
        setKSData(table, getKSData(table, j), j - 1);
    }
    table->tData.csize--;
    setTableParams(table, 0);
}

static void updatePointers(Table *table, KSData *ksData, int release, NData nData, NData pNData) {
    if (!release || (!pNData.ptr && !nData.ptr)) ksData->ptr = 0;
    else if (pNData.ptr) {
        pNData.ptr = nData.ptr;
        fwrite(&pNData, sizeof(pNData), 1, table->file);
    } else ksData->ptr = nData.ptr;
}
