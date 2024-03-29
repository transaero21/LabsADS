#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/utils.h"

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


int isPositive(int i) {
    return i > 0;
}

void pressEnterToContinue() {
    printf("\nPress Enter (Return) to continue...");
    if (getchar() != '\n') {
        scanf("%*[^\n]");
        scanf("%*c");
    }
}
