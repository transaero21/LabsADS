#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/menu.h"
#include "include/utils.h"

void initMenu(Table *table) {
    int opt, ret;

    do {
        system("clear");
        printf(MENU_OPTIONS);
        ret = readInt(&opt, isMenuOption);
        if (ret) {
            system("clear");
            ret = opts[opt - 1](table);
            if (ret) pressEnterToContinue();
        }
    } while (ret);
}

int mInsert(Table *table) {
    char *key = NULL, *info = NULL;
    int ret = 0;

    printf("Enter a key for the new element: ");
    if ((key = readLine(stdin))) {
        printf("Enter a info for the new element: ");
        if ((info = readLine(stdin))) {
            ret = insert(table, key, info);
            if (ret) printf("\nLine was successfully written to the table!\n");
            else printf("\nTable overflow, cannot write new record!\n");
            free(info);
            ret = 1;
        }
        free(key);
    }

    return ret;
}

int mDelete(Table *table) {
    char *key = NULL;
    int release = 0, ret = 0;

    printf("Enter a key of the element to be removed: ");
    if ((key = readLine(stdin))) {
        printf("Enter a release of the element to be removed (0 for all): ");
        if (readInt(&release, NULL)) {
            ret = delete(table, key, release);
            if (ret) printf("\nSuccessfully deleted from the table!\n");
            else printf("\nNothing found with these parameters!\n");
            ret = 1;
        }
        free(key);
    }
    return ret;
}

int mSearch(Table *table) {
    char *key = NULL;
    int release = 0, ret = 0;

    printf("Enter a key of the element you are looking for: ");
    if ((key = readLine(stdin))) {
        printf("Enter a release of the element you are looking for (0 for all): ");
        if (readInt(&release, NULL)) {
            printf("\n");
            initPrint();
            search(table, key, release, printLine);
            finalizePrint();
            ret = 1;
        }
        free(key);
    }

    return ret;
}

int mImport(Table *table) {
    char *line, **keys = NULL, **info = NULL, *filename = NULL;
    int size = 0;
    FILE *file;

    if ((getFile(&file, &filename, OPEN)) == FAILED) return 0;
    for (; (line = readLine(file)); free(line)) {
        char *tkn = strtok(line, ",");
        for (int i = 0; i < 2; i++) {
            if (i % 2 != 1) {
                if (tkn) {
                    keys = realloc(keys, ++size * sizeof(char *));
                    keys[size - 1] = strdup(tkn);
                } else break;
            } else {
                if (tkn) {
                    info = realloc(info, size * sizeof(char *));
                    info[size - 1] = strdup(tkn);
                } else {
                    free(keys[--size]);
                    keys = realloc(keys, size * sizeof(char *));
                }
            }
            tkn = strtok(NULL, ",");
        }
    }
    fclose(file), free(line);

    int ret = import(table, keys, info, size);
    printf("\nImported %d lines from %s file!\n", ret, filename);
    for (int i = 0; i < size; i++) {
        free(keys[i]), free(info[i]);
    }
    free(keys), free(info), free(filename);

    return 1;
}

int mPrint(Table *table) {
    initPrint();
    print(table, printLine);
    finalizePrint();
    return 1;
}

int mExit(__attribute__((unused)) Table *table) {
    return 0;
}

static int isMenuOption(int opt) {
    return 1 <= opt && opt <= 6;
}

static void initPrint() {
    printf("┌────────────┬────────────┬────────────┐\n");
    printf("│ Key        │ Info       │ Release    │\n");
}

static void printLine(const char *key, const char *info, int release) {
    printf("├────────────┼────────────┼────────────┤\n");
    printf("│ %-10s │ %-10s │ %-10d │\n", key, info, release);
}

static void finalizePrint() {
    printf("└────────────┴────────────┴────────────┘\n");
}
