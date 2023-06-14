#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../tree.h"
#include "../../include/utils.h"

static void localInsert(Tree *tree, char *key);

int concurrence(FILE *file, void (*println)(Node *, int)) {
    Tree *tree = initTree();

    char *line, *sep =" \t\r\n\v\f";
    for (; strlen((line = readLine(file))); free(line)) {
        char *key = strtok(line, sep);
        while(key) {
            localInsert(tree, key);
            key = strtok(NULL, sep);
        }
    }
    free(line);

    int i = traverse(tree, println);
    destroyTree(tree);
    return i;
}

static void localInsert(Tree *tree, char *key) {
    Node *node = tree->root, *next = node;
    if (!tree->root) {
        tree->root = initNode(strdup(key), 1, NULL);
    } else {
        int cmp;
        while(next && (cmp = strcmp(key, next->key)) ) {
            node = next;
            next = cmp < 0 ? node->left : node->right;
        }
        if (next) next->info->value++;
        else {
            next = initNode(strdup(key), 1, node);
            if (cmp < 0) node->left = next;
            else node->right = next;
        }
    }
}
