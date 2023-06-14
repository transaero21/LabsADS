#include "../table.h"

int import(Table *table, char **keys, char **info, int size) {
    Table *lTable = temporaryTable(table, table->tData.msize);

    int i = 0;
    for (int j = 0; j < size; i++, j++) {
        if (!insert(lTable, keys[j], info[j])) i--;
    }

    rename(lTable->filename, table->filename);
    table->file = freopen(table->filename, "r+b", table->file);
    table->tData = lTable->tData;
    destroyTable(lTable);

    return i;
}