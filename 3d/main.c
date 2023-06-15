#include <stdio.h>

#include "include/menu.h"

int main() {
    Table *table = initTable();

    if (table) {
        initMenu(table);
        destroyTable(table);
    }
    printf("\nGoodbye...\n");

    return 0;
}
