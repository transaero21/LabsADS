#include <stdlib.h>
#include <string.h>

#include "../tree.h"

int min(int x, int y) {
    if (x == 0 && y != 0) return y;
    if (x != 0 && y == 0) return x;
    return (x + y - abs(x - y)) / 2;
}

Node **diff(Tree *tree, const char *key) {
    Node *node = tree->root, **nodeArr = NULL;
    int cmp;

    while (node && (cmp = strcmp(key, node->key))) {
        node = cmp < 0 ? node->left : node->right;
    }

    if (node) {
        int pCmp = node->parent ? -strcmp(node->key, node->parent->key) : 0;
        int rCmp = node->right ? -strcmp(node->key, node->right->key) : 0;
        int lCmp = node->left ? strcmp(node->key, node->left->key) : 0;
        cmp = min(min(pCmp, rCmp), lCmp);
        if (cmp) {
            if (cmp == pCmp) appendNodeArr(&nodeArr, node->parent);
            if (cmp == rCmp) appendNodeArr(&nodeArr, node->right);
            if (cmp == lCmp) appendNodeArr(&nodeArr, node->left);
        }
        splay(tree, node);
    }

    return nodeArr;
}
