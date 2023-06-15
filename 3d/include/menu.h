#ifndef MENU_H
#define MENU_H

#include <stdlib.h>

#include "../include/utils.h"
#include "../table/table.h"

#define MENU_OPTIONS                            \
    "Menu options\n"                            \
    "\n"                                        \
    "┌───────────────────────────────────┐\n"   \
    "│ 1. Insert element in table        │\n"   \
    "├───────────────────────────────────┤\n"   \
    "│ 2. Remove element from table      │\n"   \
    "├───────────────────────────────────┤\n"   \
    "│ 3. Search for element in table    │\n"   \
    "├───────────────────────────────────┤\n"   \
    "│ 4. Import data from CSV file      │\n"   \
    "├───────────────────────────────────┤\n"   \
    "│ 5. Remove old records from table  │\n"   \
    "├───────────────────────────────────┤\n"   \
    "│ 6. Print current table            │\n"   \
    "├───────────────────────────────────┤\n"   \
    "│ 7. Exit program                   │\n"   \
    "└───────────────────────────────────┘\n"   \
    "\n"                                        \
    "Type your option from 1 to 6, then press ENTER: "

void initMenu(Table *table);

int mInsert(Table *table);
int mDelete(Table *table);
int mSearch(Table *table);
int mImport(Table *table);
int mClean(Table *table);
int mPrint(Table *table);
int mExit(Table *table);

static int (*opts[])(Table *) = {
        mInsert,
        mDelete,
        mSearch,
        mImport,
        mClean,
        mPrint,
        mExit
};

static int isMenuOption(int opt);
static void initPrint();
static void printLine(int key, const char *info, int release);
static void finalizePrint();

#endif //MENU_H
