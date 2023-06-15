#include "include/utils.h"

#include <stdlib.h>
#include <string.h>

char *readLine(FILE *file) {
    char *ret = calloc(1, sizeof(char)), buf[MAX_BUF];
    int i;

    do {
        buf[0] = '\0';
        i = fscanf(file, "%63[^\n]", buf);
        if (i > 0 || file != stdin) {
            int newSize = (int) strlen(ret) + (int) strlen(buf) + 1;
            ret = realloc(ret, newSize * sizeof(char));
            strcat(ret, buf);
        }
        if (i == EOF) {
            if (strlen(ret) && file != stdin) continue;
            free(ret);
            ret = NULL;
        }
    } while (i > 0);
    fscanf(file, "%*c");

    return ret;
}

int readInt(int *i, int(*verify)(int)) {
    int ret;

    do {
        ret = scanf("%d", i);
        if (ret == EOF) return 0;
        else if ((ret = !ret || (verify ? !verify(*i) : 0))) {
            printf("Incorrect input, try again: ");
        }
        scanf("%*[^\n]");
        scanf("%*c");
    } while (ret);

    return 1;
}

enum FStatus getFile(FILE **file, char **filename, enum FRequest request) {
    int ret = FAILED;

    printf("Enter path to the file to work with: ");
    do {
        *filename = readLine(stdin);
        if (!*filename) break;
        if (!(*file = fopen(*filename, "r+b"))) {
            if (request != CREATE || !(*file = fopen(*filename, "w+b"))) {
                printf("Failed to open or create %s, try again: ", *filename);
            } else ret = CREATED;
        } else ret = OPENED;
        if (ret == FAILED) {
            free(*filename), *filename = NULL;
        }
    } while (!*file);

    return ret;
}

int stringToInt(const char* string, int *i) {
    int j = string[0] != '-' ? 0 : 1, isMinus = j;
    *i = 0;

    for (; string[j]; j++) {
        if ('0' > string[j] || string[j] > '9') return 0;
        *i *= 10;
        *i += string[j] - '0';
    }
    if (isMinus) *i *= -1;

    return 1;
}

int isPositive(int i) {
    return i > 0;
}

int isNotNegative(int i) {
    return i > -1;
}

void pressEnterToContinue() {
    printf("\nPress Enter (Return) to continue...");
    if (getchar() != '\n') {
        scanf("%*[^\n]");
        scanf("%*c");
    }
}