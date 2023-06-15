#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    char *key = NULL, *value = NULL;
    int ret = 0;

    printf("Enter a key for the new element: ");
    if ((key = readLine(stdin))) {
        printf("Enter a info for the new element: ");
        if ((value = readLine(stdin))) {
            *time = clock();
            char *old = insert(tree, key, value, 1);
            *time = clock() - *time;
            if (old) printf("\nValue was replaced, old %s\n", old);
            else printf("\nElement was successfully inserted\n");
            ret = 1;
        }
    }
    free(key), free(value);

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

int mTraverse(Tree *tree, clock_t *time) {
    char *key = NULL;
    int ret = 0;

    printf("Enter a key to output large items (empty for none): ");
    if ((key = readLine(stdin))) {
        *time = clock();
        ret = traverse(tree, strlen(key) ? key : NULL, printNode);
        if (!ret) printf("\nNothing to display!\n");
        *time = clock() - *time;
        ret = 1;
        free(key);
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
        } else printf("\nNothing found for key %s!\n", key);
        free(nodeArr), free(key);
        ret = 1;
    }

    return ret;
}

int mPrint(Tree *tree, clock_t *time) {
    *time = clock();
    print(tree);
    *time = clock() - *time;
    return 1;
}

int mImport(Tree *tree, clock_t *time) {
    char *filename;
    FILE *file;

    if (!(getFile(&file, &filename))) return 0;
    *time = clock();
    import(tree, file);
    *time = clock() - *time;
    fclose(file), free(filename);

    return 1;
}

int mFinder(__attribute__((unused)) Tree *tree, clock_t *time) {
    Tree *lTree = initTree();
    char *filename, *line, *word, *sep = " \t\r\n\v\f";
    FILE *file;

    if (!(getFile(&file, &filename))) return 0;
    for (int i = 1; (line = readLine(file)); free(line), i++) {
        char *orig = strdup(line), *key = strtok(line, sep);
        while (key) {
            int oP = (int) (strstr(orig, key) - &orig[0] + 1);
            char *l = intToString(i), *o = intToString(oP);
            appendString(&l, ": "), appendString(&l, o);
            insert(lTree, key, l, 0);
            free(l), free(o),
            key = strtok(NULL, sep);
        }
        free(orig);
    }

    int isOk = 1, ret = 1;
    *time = clock();
    do {
        printf("\n>>> ");
        word = readLine(stdin);
        if (!word) ret = 0;
        if (ret && (isOk = strlen(word) != 0)) {
            Node **nodeArr = search(lTree, word);
            if (nodeArr) printf("%s: %s\n", filename, nodeArr[0]->value);
            else printf("Nothing found for word %s!\n", word);
            free(nodeArr);
        }
        free(word);
    } while (isOk);
    destroyTree(lTree), free(filename), fclose(file);
    *time = clock() - *time;

    return ret;
}

int mExit() {
    return 0;
}

static int isMenuOption(int opt) {
    return 1 <= opt && opt <= 9;
}

static void printNode(Node *node, int i) {
    printf("%d. %s (%s)\n", i, node->key, node->value);
}