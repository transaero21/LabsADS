#include <stdlib.h>
#include <string.h>

#include "../tree.h"

int max(int a, int b) {
    return (a + b + abs(a - b)) / 2;
}

Node **diff(Tree *tree, const char *key) {
    Node *node = tree->root, **nodeArr = NULL;
    int cmp;

    while (node && (cmp = strcmp(key, node->key))) {
        node = cmp < 0 ? node->left : node->right;
    }

    if (node) {
        Node *left = getLeftestNode(tree->root), *right = getRightestNode(tree->root);
        int rCmp = -strcmp(node->key, right->key), lCmp = strcmp(node->key, left->key);
        cmp = max(rCmp, lCmp);
        if (cmp) {
            if (cmp == rCmp) appendNodeArr(&nodeArr, right);
            else if (cmp == lCmp) appendNodeArr(&nodeArr, left);
        }
    }

    return nodeArr;
}
