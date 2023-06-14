#include <stdlib.h>
#include <string.h>

#include "../tree.h"

int delete(Tree *tree, const char *key) {
    Node *node = tree->root;
    int cmp;

    while (node && (cmp = strcmp(key, node->key))) {
        node = cmp < 0 ? node->left : node->right;
    }

    if (node) {
        splay(tree, node);
        Node *nRoot = getRightestNode(tree->root->left);
        if (nRoot) {
            tree->root->left->parent = NULL;
            Tree lTree = {.root = tree->root->left};
            splay(&lTree, nRoot);
            nRoot->right = tree->root->right;
            nRoot->right->parent = nRoot;
            tree->root = nRoot;
        } else tree->root = tree->root->right;
        free(node->key), free(node->value), free(node);
    }

    return !cmp;
}
