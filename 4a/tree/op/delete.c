#include <stdlib.h>
#include <string.h>

#include "../tree.h"

int delete(Tree *tree, const char *key) {
    Node *node = tree->root;
    int cmp;

    while (node && (cmp = strcmp(key, node->key))) {
        node = cmp < 0 ? node->left : node->right;
    }

    if (!node) return 0;
    if (node->info->next) {
        Info *pInfo = node->info, *info = pInfo->next;
        for (; info->next; pInfo = info, info = info->next);
        pInfo->next = NULL;
        free(info);
    } else {
        free(node->info);
        if (!node->left && !node->right) {
            if (!node->parent) tree->root = NULL;
            else if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        } else {
            Node *rNode = node->left ? getRightestNode(node->left) : getLeftestNode(node->right);
            if (!node->parent) tree->root = rNode;
            else if (node->parent->left == node) node->parent->left = rNode;
            else if (node->parent->right == node) node->parent->right = rNode;
            if (node->left) node->left->parent = rNode;
            if (node->right) node->right->parent = rNode;
            if (rNode->parent != node) {
                if (node->left) {
                    rNode->parent->right = rNode->left;
                    if (rNode->left) rNode->left->parent = rNode->parent;
                } else if (node->right) {
                    rNode->parent->left = rNode->right;
                    if (rNode->right) rNode->right->parent = rNode->parent;
                }
            }
            rNode->parent = node->parent;
            if (node->right != rNode) rNode->right = node->right;
            if (node->left != rNode) rNode->left = node->left;
        }
        free(node->key);
        free(node);
    }

    return 1;
}
