#include <string.h>

#include "../table.h"

int insert(Table *table, const char *key, const char *value) {
    int j = searchForIndex(table, 0, table->csize - 1, key, 1), release = 1;
    if (!compareKeys(table, j, key)) {
        release += table->ks[j].node->release;
    } else if (table->csize >= table->msize) return 0;
    else {
        for (int i = table->csize++; i > j; i--) {
            table->ks[i] = table->ks[i - 1];
        }
        table->ks[j].key = strdup(key);
        table->ks[j].node = NULL;
    }
    table->ks[j].node = initNode(table->ks[j].node, release, value);
    return 1;
}
