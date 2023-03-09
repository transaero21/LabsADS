#include <stdio.h>
#include <stdlib.h>

#include "include/utils.h"

int getInt(int *i, int(*verify)(int)) {
    int ret;

    do {
        ret = scanf("%d", i);
        if (ret == EOF) return 0;
        else if ((ret = !ret || (verify ? !verify(*i) : 0))) {
            scanf("%*[^\n]");
            printf("Incorrect input, try again: ");
        }
    } while (ret);

    return 1;
}

int isPositive(int i) {
    return i > 0;
}

void pressEnterToContinue() {
    printf("\nSuccess, press ENTER to continue...");
    getchar();
    scanf("%*[^\n]");
}

void clearOutput() {
    system("clear");
}