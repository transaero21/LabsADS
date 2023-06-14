#include "../table.h"

void print(Table *table, void (*println)(const char *, const char *, int)) {
    for (int i = 0; i < table->csize; i++) {
        for (Node *node = table->ks[i].node; node; node = node->next) {
            println(table->ks[i].key, node->info, node->release);
        }
    }
}