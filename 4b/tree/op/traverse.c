#include <string.h>

#include "../tree.h"

static void lPrintNode(Node *node, int *i, const char *key, void (*println)(Node *, int), int pass);

int traverse(Tree *tree, const char *key, void (*println)(Node *, int)) {
    int i = 0;
    if (tree->root) lPrintNode(tree->root, &i, key, println, !key);
    return i;
}

static void lPrintNode(Node *node, int *i, const char *key, void (*println)(Node *, int), int pass) {
    int cmp = pass;
    if (pass || (cmp = strcmp(key, node->key) < 0)) {
        if (node->left) lPrintNode(node->left, i, key, println, pass ? 1 : 0);
    }
    if (node->right) lPrintNode(node->right, i, key, println, cmp ? 1 : 0);
    if (pass || cmp) println(node, ++(*i));
}






