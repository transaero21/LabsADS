#include "../table.h"

int import(Table *table, char **keys, char **info, int size) {
    int i = 0;
    for (; table->ks[0].key; deleteByIndex(table, 0, 1));
    for (int j = 0; j < size; i++, j++) {
        if (!insert(table, keys[j], info[j])) i--;
    }
    return i;
}
