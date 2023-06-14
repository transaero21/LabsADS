#include <string.h>

#include "../tree.h"

char *insert(Tree *tree, const char *key, const char *value, int override) {
    Node *prev = tree->root, *node = prev;
    char *ret = NULL;
    int cmp;

    if (!tree->root) {
        tree->root = initNode(strdup(key), strdup(value), NULL);
    } else {
        while(node && (cmp = strcmp(key, node->key)) ) {
            prev = node;
            node = cmp < 0 ? prev->left : prev->right;
        }
        if (cmp) {
            node = initNode(strdup(key), strdup(value), prev);
            if (cmp < 0) prev->left = node;
            else prev->right = node;
        } else if (override) {
            node = prev;
            ret = node->value;
            node->value = strdup(value);
        }
        splay(tree, node);
    }

    return ret;
}
