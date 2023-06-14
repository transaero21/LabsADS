#include <string.h>

#include "../tree.h"

void insert(Tree *tree, const char *key, int value) {
    Node *node = tree->root, *next = node;
    int cmp;

    if (!tree->root) {
        tree->root = initNode(strdup(key), value, NULL);
    } else {
        while(next && (cmp = strcmp(key, next->key)) ) {
            node = next;
            next = cmp < 0 ? node->left : node->right;
        }
        if (!cmp) {
            Info *info = next->info;
            while (info->next) info = info->next;
            info->next = initInfo(value);
        } else  {
            next = initNode(strdup(key), value, node);
            if (cmp < 0) node->left = next;
            else node->right = next;
        }
    }
}
