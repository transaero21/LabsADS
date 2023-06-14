#include "../tree.h"

static void printNode(Node *node, void (*println)(Node *, int), int *i);

int traverse(Tree *tree, void (*println)(Node *, int)) {
    int i = 0;
    printNode(tree->root, println, &i);
    return i != 0;
}

static void printNode(Node *node, void (*println)(Node *, int), int *i) {
    if (node->left) printNode(node->left, println, i);
    if (node->right) printNode(node->right, println, i);
    println(node, ++(*i));
}


