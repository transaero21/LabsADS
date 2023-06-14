#include <stdio.h>

#include "include/menu.h"

int main() {
    Tree *tree = initTree();

    if (tree) {
        initMenu(tree);
        destroyTree(tree);
    }
    printf("\nGoodbye...\n");

    return 0;
}
