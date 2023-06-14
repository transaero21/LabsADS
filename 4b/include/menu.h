#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <time.h>

#include "../tree/tree.h"

#define MENU_OPTIONS                         \
    "Menu options\n"                         \
    "\n"                                     \
    "┌────────────────────────────────┐\n"   \
    "│ 1. Insert element in tree      │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 2. Remove element from tree    │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 3. Tree traversal              │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 4. Search element in tree      │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 5. Search element by diff      │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 6. Print current tree          │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 7. Import data from file       │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 8. String finder in file       │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 9. Exit program                │\n"   \
    "└────────────────────────────────┘\n"   \
    "\n"                                     \
    "Type your option from 1 to 9, then press ENTER: "

void initMenu(Tree *tree);

int mInsert(Tree *tree, clock_t *time);
int mDelete(Tree *tree, clock_t *time);
int mTraverse(Tree *tree, clock_t *time);
int mSearch(Tree *tree, clock_t *time);
int mDiff(Tree *tree, clock_t *time);
int mPrint(Tree *tree, clock_t *time);
int mImport(Tree *tree, clock_t *time);
int mFinder(Tree *tree, clock_t *time);
int mExit();

static int (*opts[])(Tree *, clock_t *) = {
        mInsert,
        mDelete,
        mTraverse,
        mSearch,
        mDiff,
        mPrint,
        mImport,
        mFinder,
        mExit
};

static int isMenuOption(int opt);
static void printNode(Node *node, int i);

#endif //MENU_H
