#include <stdlib.h>

#include "tree.h"

Tree *initTree() {
    return calloc(1, sizeof(Tree));
}

void destroyTree(Tree *tree) {
    destroyNode(tree->root);
    free(tree);
}

Node *initNode(char *key, char *value, Node *parent) {
    Node *node = calloc(1, sizeof(Node));
    node->value = value;
    node->parent = parent;
    node->key = key;
    return node;
}

void destroyNode(Node *node) {
    if (node) {
        if (node->left) destroyNode(node->left);
        if (node->right) destroyNode(node->right);
        free(node->key), free(node->value), free(node);
    }
}

Node *getRightestNode(Node *node) {
    if (node) while (node->right) node = node->right;
    return node;
}

void appendNodeArr(Node ***nodeArr, Node *node) {
    int i = 0;
    if (*nodeArr) while ((*nodeArr)[i]) i++;
    *nodeArr = realloc(*nodeArr, (i + 2) * sizeof(Node *));
    (*nodeArr)[i] = node;
    (*nodeArr)[i + 1] = NULL;
}

void splay(Tree *tree, Node *node) {
    while(node->parent) {
        int isLeft = node->parent->left == node;
        if (!node->parent->parent) {
            if (isLeft) {
                leftRotate(node);
            } else {
                rightRotate(node);
            }
        } else {
            int isPLeft = node->parent->parent->left == node->parent;
            if (isLeft) {
                if (isPLeft) {
                    leftRotate(node->parent);
                    leftRotate(node);
                } else {
                    leftRotate(node);
                    rightRotate(node);
                }
            } else {
                if (isPLeft) {
                    rightRotate(node);
                    leftRotate(node);
                } else {
                    rightRotate(node->parent);
                    rightRotate(node);
                }
            }
        }
    }
    tree->root = node;
}

void rotate(Node *node) {
    Node *pNode = node->parent->parent;
    if (pNode) {
        if (pNode->left == node->parent) {
            pNode->left = node;
        } else {
            pNode->right = node;
        }
    }
    node->parent->parent = node;
    node->parent = pNode;
}

void leftRotate(Node *node) {
    node->parent->left = node->right;
    if (node->right) {
        node->right->parent = node->parent;
    }
    node->right = node->parent;
    rotate(node);
}

void rightRotate(Node *node) {
    node->parent->right = node->left;
    if (node->left) {
        node->left->parent = node->parent;
    }
    node->left = node->parent;
    rotate(node);
}

