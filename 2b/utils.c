#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/utils.h"

char *readLine() {
    char *ret = calloc(1, sizeof(char)), *buf = malloc(64 * sizeof(char));
    int i;

    do {
        i = scanf("%63[^\n]", buf);
        if (i > 0) {
            ret = realloc(ret, (strlen(ret) + strlen(buf) + 1) * sizeof(char));
            strcat(ret, buf);
        } else if (i == EOF) {
            free(ret);
            ret = NULL;
        }
    } while (i > 0);
    free(buf);

    return ret;
}

int stringToInt(const char* string, int *i) {
    int j = string[0] != '-' ? 0 : 1, isMinus = j;

    for (; string[j]; j++) {
        if ('0' > string[j] || string[j] > '9') {
            printf("String \"%s\" can't be an integer, bad char at %d!\n", string, j);
            return 0;
        }
        *i *= 10;
        *i += string[j] - '0';
    }
    if (isMinus) *i *= -1;

    return 1;
}

int getInt(int *i, int(*verify)(int)) {
    int ret;

    do {
        printf("> ");
        ret = scanf("%d", i);
        if (ret == EOF) return 0;
        else if ((ret = !ret || (verify ? !verify(*i) : 0))) {
            scanf("%*[^\n]");
            printf("Invalid line format, try again!\n");
        }
    } while (ret);

    return 1;
}

void appendString(char **target, const char *string) {
    int space = !!strlen(*target);
    size_t size = strlen(*target) + strlen(string) + space + 1;
    *target = realloc(*target, size * sizeof(char));
    if (space) strcat(*target, " ");
    strcat(*target, string);
}

void dropStringBeforeChar(char **target, char symbol) {
    char *new = strchr(*target, symbol);
    if (new) {
        memmove(*target, new + sizeof(char), strlen(new));
    } else (*target)[0] = '\0';
    *target = realloc(*target, (strlen(*target) + 1) * sizeof(char));
}

int pressEnterToContinue() {
    printf("\nPress Enter (Return) to continue...");
    getchar();
    scanf("%*[^\n]");
    return scanf("%*c") + 1;
}

void clearOutput() {
    system("clear");
}

int isPositive(int i) {
    return i > 0;
}