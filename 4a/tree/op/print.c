#include <stdio.h>
#include <stdlib.h>

#include "../tree.h"

static void lPrintNode(Node *node, FILE *file, int *cluster);
static FILE *setupGraphviz();
static void nodeGraphviz(Node *node, Node *parent, FILE *file, int *cluster);
static void finalizeGraphviz(FILE *file);

#define DOT_FILE "/tmp/tree.dot"
#define PNG_FILE "/tmp/tree.png"

#define DOT_CMD ("dot -Tpng " DOT_FILE " -o " PNG_FILE)
#define IMG_CMD ("catimg " PNG_FILE)

int print(Tree *tree) {
    int cluster = 0;

    if (tree->root) {
        FILE *file = setupGraphviz();
        if (file) {
            lPrintNode(tree->root, file, &cluster);
            finalizeGraphviz(file);
        }
    }

    return cluster != 0;
}

static void lPrintNode(Node *node, FILE *file, int *cluster) {
    nodeGraphviz(node, node->parent, file, cluster);
    if (node->left) lPrintNode(node->left, file, cluster);
    if (node->right) lPrintNode(node->right, file, cluster);
}

static void nodeGraphviz(Node *node, Node *parent, FILE *file, int *cluster) {
    if (parent) {
        char *point = parent->left == node ? "sw" : "se";
        fprintf(file, "\t\"%s\":%s->\"%s\"\n", parent->key, point, node->key);
    }
    fprintf(file, "\tsubgraph cluster_%d {\n", *cluster);
    fprintf(file, "\t\tnode [shape=box, style=filled]\n");
    Info *info = node->info, *prev = NULL;
    for (int i = 0; info; prev = info, info = info->next, i++) {
        fprintf(file, "\t\t\"%s_%d_%d\" [label=%d]", node->key, info->value, i, info->value);
        if (prev) fprintf(file, "\t\t\"%s_%d_%d\"->\"%s_%d_%d\"",
                          node->key, prev->value, i - 1, node->key, info->value, i);
    }
    fprintf(file, "\n");
    fprintf(file, "\t\tcolor=blue\n");
    fprintf(file, "\t}\n");
    fprintf(file, "\t\"%s\":s->\"%s_%d_0\" [lhead=cluster_%d]\n",
            node->key, node->key, node->info->value, (*cluster)++);
}

static FILE *setupGraphviz() {
    if (!system("which dot > /dev/null 2>&1")) {
        FILE *file = fopen(DOT_FILE, "w");
        if (file) {
            fprintf(file, "digraph tree {\n");
            fprintf(file, "\tnode [shape=ellipse]\n");
            return file;
        } else fprintf(stderr, "Failed to print: failed to create file!\n");
    } else fprintf(stderr, "Failed to print: \"dot\" not found!\n");
    return NULL;
}

static void finalizeGraphviz(FILE *file) {
    fprintf(file, "}\n");
    fflush(file);
    system(DOT_CMD);
    if (!system("which catimg > /dev/null 2>&1")) {
        system(IMG_CMD);
    } else {
        fprintf(stderr, "Failed to print: \"catimg\" not found!\n");
    }
    fclose(file);
}