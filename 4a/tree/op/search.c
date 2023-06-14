#include <string.h>

#include "../tree.h"

Node **search(Tree *tree, const char *key) {
    Node *node = tree->root, **nodeArr = NULL;
    int cmp;

    while (node && (cmp = strcmp(key, node->key))) {
        node = cmp < 0 ? node->left : node->right;
    }
    if (node) appendNodeArr(&nodeArr, node);

    return nodeArr;
}