#include <stdlib.h>
#include <string.h>

#include "../table.h"

void search(Table *table, const char *key, int release, void (*println)(const char *, const char *, int)) {
    int j = -1;
    Node *node = getNodeByParam(table, key, release, &j);

    Table *lTable = calloc(1, sizeof(Table));
    lTable->ks = calloc(1, sizeof(KeySpace));
    lTable->msize = 1;
    lTable->ks[0].key = strdup(key);
    lTable->csize = 1;

    for (Node *pNode = NULL; node; node = node->next) {
        if (!release || release == node->release) {
            Node *iNode = calloc(1, sizeof(Node));
            iNode->release = node->release;
            iNode->info = strdup(node->info);
            if (!pNode) lTable->ks[0].node = iNode;
            else pNode->next = iNode;
            pNode = iNode;
        }
        if (release) break;
    }

    print(lTable, println);
    destroyTable(lTable);
}