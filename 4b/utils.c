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

int getFile(FILE **file, char **filename) {
    int ret = 0;

    printf("Enter path to the file to work with: ");
    do {
        *filename = readLine(stdin);
        if (!*filename) break;
        if (!(*file = fopen(*filename, "r"))) {
            printf("Failed to open %s, try again: ", *filename);
        } else ret = 1;
        if (!ret) free(*filename), *filename = NULL;
    } while (!*file);

    return ret;
}

char *intToString(int i) {
    int length = snprintf(NULL, 0, "%d", i);
    char *string = malloc((length + 1) * sizeof(char));
    sprintf(string, "%d", i);
    return string;
}

void appendString(char **target, const char *string) {
    size_t size = strlen(*target) + strlen(string) + 1;
    *target = realloc(*target, size * sizeof(char));
    strcat(*target, string);
}

void pressEnterToContinue(clock_t time) {
    printf("\n%Lf spent, press Enter (Return) to continue...", (long double) time / CLOCKS_PER_SEC);
    if (getchar() != '\n') {
        scanf("%*[^\n]");
        scanf("%*c");
    }
}
