#ifndef TREE_H
#define TREE_H

#include <stdio.h>

typedef struct Node {
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    char *key;
    char *value;
} Node;

typedef struct Tree {
    Node *root;
} Tree;

Tree *initTree();
void destroyTree(Tree *tree);

char *insert(Tree *tree, const char *key, const char *value, int override);
int delete(Tree *tree, const char *key);
Node **search(Tree *tree, const char *key);
Node **diff(Tree *tree, const char *key);
void import(Tree *tree, FILE *file);
void print(Tree *tree);
int traverse(Tree *tree, const char *key, void (*println)(Node *, int));

Node *initNode(char *key, char *value, Node *parent);
void destroyNode(Node *node);

void leftRotate(Node *node);
void rightRotate(Node *node);
void splay(Tree *tree, Node *node);

Node *getRightestNode(Node *node);

void appendNodeArr(Node ***nodeArr, Node *node);

#endif //TREE_H
