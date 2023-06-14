#ifndef TREE_H
#define TREE_H

#include <stdio.h>

typedef struct Info {
    int value;
    struct Info *next;
} Info;

typedef struct Node {
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    char *key;
    Info *info;
} Node;

typedef struct Tree {
    Node *root;
} Tree;

// Global operations
void insert(Tree *tree, const char *key, int value);
int delete(Tree *tree, const char *key);
Node **search(Tree *tree, const char *key);
Node **diff(Tree *tree, const char *key);
int concurrence(FILE *file, void (*println)(Node *, int));
int import(Tree *tree, FILE *file);
int print(Tree *tree);
int traverse(Tree *tree, void (*println)(Node *, int));

// Manage Tree instances
Tree *initTree();
void destroyTree(Tree *tree);

// Manage Node instances
Node *initNode(char *key, int value, Node *parent);
void destroyNode(Node *node);

// Manage Info instances
Info *initInfo(int value);
void destroyInfo(Info *info);

// Miscellaneous
Node *getLeftestNode(Node *node);
Node *getRightestNode(Node *node);
void appendNodeArr(Node ***nodeArr, Node *node);

#endif //TREE_H
