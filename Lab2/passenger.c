#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/passenger.h"
#include "include/utils.h"
#include "include/queue.h"

Passenger *initPassenger() {
    Passenger *pax = calloc(1, sizeof(Passenger));
    pax->id = calloc(1, sizeof(Passenger));
    return pax;
}

void freePassenger(Passenger *pax) {
    free(pax->id);
    free(pax);
}

int readPassengers(Queue *queue, int *size) {
    int isErr = !getInt(size, isPositive);

    if (!isErr) {
        char *line = readLine(), *sptr = NULL, *tkn = strtok_r(line, " ", &sptr);

        if (!line) isErr = 1;
        while (tkn && !isErr) {
            Passenger *pax = initPassenger();
            if (stringToPassenger(tkn, pax)) {
                if (!push(queue, pax)) {
                    printf("ignoring %s!\n", tkn);
                    freePassenger(pax);
                }
            }
            else freePassenger(pax);
            tkn = strtok_r(NULL, " ", &sptr);
        }
        free(line);
    }

    return !isErr;
}

static int stringToPassenger(const char *string, Passenger *pax) {
    char *dup = strdup(string), *sptr = NULL, *tkn = strtok_r(dup, "/", &sptr);
    int isErr = 0;

    for (int i = 0; i < 3; i++) {
        if (tkn) {
            switch (i) {
                case 0:
                    appendString(&pax->id, tkn);
                    break;
                case 1:
                    if (!stringToInt(tkn, &pax->ta) || !isPositive(pax->ta)) isErr = 1;
                    break;
                case 2:
                    if (!stringToInt(tkn, &pax->ts) || !isPositive(pax->ta)) isErr = 1;
                    break;
                default:
                    printf("Unexpected failure!\n");
            }
        } else isErr = 1;
        if (isErr) {
            printf("Failed to parse %s, ignoring!\n", string);
            break;
        }
        tkn = strtok_r(NULL, "/", &sptr);
    }
    free(dup);

    return !isErr;
}
