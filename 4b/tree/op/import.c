#include <stdlib.h>
#include <string.h>

#include "../tree.h"
#include "../../include/utils.h"

void import(Tree *tree, FILE *file) {
    char *line, *key, *value;
    int i = 0, j = 0;

//    if (tree->root) {
//        destroyNode(tree->root);
 //       tree->root = NULL;
 //   }

    for (; (line = readLine(file)); free(line), i++) {
        if (i % 2 != 1) key = strdup(line);
        else {
            value = strdup(line);
            if (!insert(tree, key, value, 1)) j++;
            free(value), free(key);
        }
    }
    if (i && i % 2) free(key);
    free(line);

    i /= 2;
    printf("\nScanned %d records, %d recorded, %d duplicated!\n", i, j, i - j);
}
