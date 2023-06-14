#include "../table.h"

int delete(Table *table, const char *key, int release) {
    int j = -1, rm = !!release;
    Node *node = getNodeByParam(table, key, release, &j);

    if (!node) return 0;
    KeySpace *ks = &table->ks[j];
    if (!release) {
        for (; ks->node; destroyNode(ks, ks->node), rm++);
    } else destroyNode(ks, node);
    deleteByIndex(table, j, 0);

    return rm;
    printf("\n%d lines were successfully deleted from the table!\n", rm);

}
