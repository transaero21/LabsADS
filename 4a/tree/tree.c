#include <stdlib.h>
#include <stdio.h>

#include "tree.h"

Tree *initTree() {
    return calloc(1, sizeof(Tree));
}

void destroyTree(Tree *tree) {
    destroyNode(tree->root);
    free(tree);
}

Node *initNode(char *key, int value, Node *parent) {
    Node *node = calloc(1, sizeof(Node));
    node->info = initInfo(value);
    node->parent = parent;
    node->key = key;
    return node;
}

void destroyNode(Node *node) {
    if (node) {
        if (node->left) destroyNode(node->left);
        if (node->right) destroyNode(node->right);
        destroyInfo(node->info);
        free(node->key), free(node);
    }
}

Info *initInfo(int value) {
    Info *info = calloc(1, sizeof(Info));
    info->value = value;
    return info;
}

void destroyInfo(Info *info) {
    do {
        Info *next = info->next;
        free(info);
        info = next;
    } while (info);
}

Node *getLeftestNode(Node *node) {
    if (node) while (node->left) node = node->left;
    return node;
}

Node *getRightestNode(Node *node) {
    if (node) while (node->right) node = node->right;
    return node;
}

void appendNodeArr(Node ***nodeArr, Node *node) {
    int i = 0;
    if (*nodeArr) while ((*nodeArr)[i]) i++;
    *nodeArr = realloc(*nodeArr, (i + (i ? 1 : 2)) * sizeof(Node *));
    (*nodeArr)[i] = node;
    (*nodeArr)[i + 1] = NULL;
}
