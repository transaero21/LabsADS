#include <stdio.h>
#include <stdlib.h>

#include "../tree.h"

static void lPrintNode(Node *node, FILE *file);
static FILE *setupGraphviz();
static void nodeGraphviz(Node *node, Node *parent, FILE *file);
static void finalizeGraphviz(FILE *file);

#define DOT_FILE "/tmp/tree.dot"
#define PNG_FILE "/tmp/tree.png"

#define DOT_CMD ("dot -Tpng " DOT_FILE " -o " PNG_FILE)
#define IMG_CMD ("catimg " PNG_FILE)

void print(Tree *tree) {
    if (tree->root) {
        FILE *file = setupGraphviz();
        if (file) {
            lPrintNode(tree->root, file);
            finalizeGraphviz(file);
        }
    } else {
        printf("Tree is empty!\n");
    }
}

static void lPrintNode(Node *node, FILE *file) {
    nodeGraphviz(node, node->parent, file);
    if (node->left) lPrintNode(node->left, file);
    if (node->right) lPrintNode(node->right, file);
}

static void nodeGraphviz(Node *node, Node *parent, FILE *file) {
    fprintf(file, "\t\"%s\" [label=\"%s\\n(%s)\"]", node->key, node->key, node->value);
    if (parent) {
        char *point = parent->left == node ? "sw" : "se";
        fprintf(file, "\t\"%s\":%s->\"%s\"\n", parent->key, point, node->key);
    }
}

static FILE *setupGraphviz() {
    if (!system("which dot > /dev/null 2>&1")) {
        FILE *file = fopen(DOT_FILE, "w");
        if (file) {
            fprintf(file, "digraph tree {\n");
            fprintf(file, "\tnode [shape=ellipse]\n");
            return file;
        } else printf("Failed to print: failed to create file!\n");
    } else printf("Failed to print: \"dot\" not found!\n");
    return NULL;
}

static void finalizeGraphviz(FILE *file) {
    fprintf(file, "}\n");
    fflush(file);
    system(DOT_CMD);
    if (!system("which catimg > /dev/null 2>&1")) {
        system(IMG_CMD);
    } else {
        printf("Failed to print: \"catimg\" not found!\n");
    }
    fclose(file);
}