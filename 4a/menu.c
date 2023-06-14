#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/menu.h"
#include "include/utils.h"

void initMenu(Tree *tree) {
    clock_t time;
    int opt, ret;

    do {
        system("clear");
        printf(MENU_OPTIONS);
        ret = readInt(&opt, isMenuOption);
        if (ret) {
            system("clear");
            ret = opts[opt - 1](tree, &time);
            if (ret) pressEnterToContinue(time);
        }
    } while (ret);
}

int mInsert(Tree *tree, clock_t *time) {
    int ret = 0, value = 0;
    char *key = NULL;

    printf("Enter a key for the new element: ");
    if ((key = readLine(stdin))) {
        printf("Enter a info for the new element: ");
        if (readInt(&value, NULL)) {
            *time = clock();
            insert(tree, key, value);
            *time = clock() - *time;
            ret = 1;
        }
        free(key);
    }

    return ret;
}


int mDelete(Tree *tree, clock_t *time) {
    char *key = NULL;
    int ret = 0;

    printf("Enter a key for the element to delete: ");
    if ((key = readLine(stdin))) {
        *time = clock();
        ret = delete(tree, key);
        *time = clock() - *time;
        if (ret) printf("\nElement was successfully deleted\n");
        else printf("\nNothing found with key %s\n", key);
        free(key);
        ret = 1;
    }

    return ret;
}

int mSearch(Tree *tree, clock_t *time) {
    char *key = NULL;
    int ret = 0;

    printf("Enter a key for the element to search: ");
    if ((key = readLine(stdin))) {
        *time = clock();
        Node **nodeArr = search(tree, key);
        *time = clock() - *time;
        if (nodeArr) {
            printf("\nThe following values were found:\n");
            for (int i = 0; nodeArr[i]; i++) {
                printNode(nodeArr[i], i + 1);
            }
            printf("\n");
        } else printf("\nNothing found with key %s!\n", key);
        free(nodeArr), free(key);
        ret = 1;
    }

    return ret;
}

int mDiff(Tree *tree, clock_t *time) {
    char *key = NULL;
    int ret = 0;

    printf("Enter a key for the element to search: ");
    if ((key = readLine(stdin))) {
        *time = clock();
        Node **nodeArr = diff(tree, key);
        *time = clock() - *time;
        if (nodeArr) {
            printf("\nThe following values were found:\n");
            for (int i = 0; nodeArr[i]; i++) {
                printNode(nodeArr[i], i + 1);
            }
            printf("\n");
        } else printf("\nNothing found for key %s!\n", key);
        free(nodeArr), free(key);
        ret = 1;
    }

    return ret;
}

int mImport(Tree *tree, clock_t *time) {
    FILE *file;

    if (!(getFile(&file))) return 0;
    *time = clock();
    int i = import(tree, file);
    *time = clock() - *time;
    printf("%d lines were successfully written to tree", i);

    return 1;
}

int mConcurrences(__attribute__((unused)) Tree *tree, clock_t *time) {
    FILE *file;

    if (!(getFile(&file))) return 0;
    *time = clock();
    int i = concurrence(file, printNode);
    *time = clock() - *time;
    if (!i) printf("File is empty, nothing to display!\n");

    return 1;
}

int mPrint(Tree *tree, clock_t *time) {
    *time = clock();
    int i = print(tree);
    *time = clock() - *time;
    if (!i) printf("Tree is empty, nothing to display!\n");
    return 1;
}

int mTraverse(Tree *tree, clock_t *time) {
    *time = clock();
    int i = traverse(tree, printNode);
    *time = clock() - *time;
    if (!i) printf("Tree is empty, nothing to display!\n");
    return 1;
}

int mExit() {
    return 0;
}

static int isMenuOption(int opt) {
    return 1 <= opt && opt <= 9;
}

static void printNode(Node *node, int i) {
    printf("%d. %s", i, node->key);
    for (Info *info = node->info; info; info = info->next) {
        printf(" -> %d", info->value);
    }
    printf("\n");
}