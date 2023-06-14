#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../tree.h"
#include "../../include/utils.h"

int import(Tree *tree, FILE *file) {
    if (tree->root) {
        destroyNode(tree->root);
        tree->root = NULL;
    }

    char *line, *key;

    int i = 0, j = 0, value = 0;
    for (; strlen((line = readLine(file))); free(line), i++) {
        if (i % 2 != 1) key = strdup(line);
        else {
            if (stringToInt(line, &value)) {
                insert(tree, key, value), j++;
            }
            free(key);
        }
    }
    if (i && i % 2) free(key);
    free(line);

    return j;
}
