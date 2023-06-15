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

static int isMenuOption(int opt) {
    return 1 <= opt && opt <= 7;
}

int mInsert(Table *table) {
    int ret = 0, key = -1;
    char *info = NULL;

    printf("Enter a key for the new element: ");
    if (readInt(&key, isNotNegative)) {
        printf("Enter a info for the new element: ");
        if ((info = readLine(stdin))) {
            insert(table, key, info);
            printf("\nLine was successfully written to the table!\n");
            free(info);
            ret = 1;
        }
    }

    return ret;
}

int mDelete(Table *table) {
    int ret = 0, key = -1, release = 0;

    printf("Enter a key of the element to be removed: ");
    if (readInt(&key, isNotNegative)) {
        printf("Enter a release of the element to be removed (0 for all): ");
        if (readInt(&release, NULL)) {
            ret = delete(table, key, release);
            if (ret) printf("\nSuccessfully deleted from the table!\n");
            else printf("\nNothing found with these parameters!\n");
            ret = 1;
        }
    }

    return ret;
}

int mSearch(Table *table) {
    int ret = 0, key = -1, release = 0;

    printf("Enter a key of the element you are looking for: ");
    if (readInt(&key, isNotNegative)) {
        printf("Enter a release of the element you are looking for (0 for all): ");
        if (readInt(&release, NULL)) {
            printf("\n");
            initPrint();
            search(table, key, release, printLine);
            finalizePrint();
            ret = 1;
        }
    }

    return ret;
}

int mImport(Table *table) {
    int *keys = NULL, size = 0, tmp = 0;
    char **info = NULL, *filename = NULL, *line;
    FILE *file;

    if ((getFile(&file, &filename, OPEN)) == FAILED) return 0;
    for (; (line = readLine(file)); free(line)) {
        char *tkn = strtok(line, ",");
        for (int i = 0; i < 2; i++) {
            if (i % 2 != 1) {
                if (tkn && stringToInt(tkn, &tmp)) {
                    keys = realloc(keys, ++size * sizeof(int));
                    keys[size - 1] = tmp;
                } else break;
            } else {
                if (tkn) {
                    info = realloc(info, size * sizeof(char *));
                    info[size - 1] = strdup(tkn);
                } else {
                    keys = realloc(keys, --size * sizeof(int));
                }
            }
            tkn = strtok(NULL, ",");
        }
    }
    fclose(file), free(line);

    import(table, keys, info, size);
    for (int i = 0; i < size; i++) free(info[i]);
    free(keys), free(info), free(filename);

    return 1;
}

int mClean(Table *table) {
    clean(table);

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

static void initPrint() {
    printf("┌────────────┬────────────┬────────────┐\n");
    printf("│ Key        │ Info       │ Release    │\n");
}

static void printLine(int key, const char *info, int release) {
    printf("├────────────┼────────────┼────────────┤\n");
    printf("│ %-10d │ %-10s │ %-10d │\n", key, info, release);
}

static void finalizePrint() {
    printf("└────────────┴────────────┴────────────┘\n");
}
