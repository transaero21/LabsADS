#include <stdio.h>
#include <stdlib.h>

#include "include/passenger.h"
#include "include/hub.h"
#include "include/utils.h"

int main() {
    Queue *queue = calloc(1, sizeof(Queue));
    int isOk = 1, size = 0;

    while (isOk) {
        clearOutput();
        isOk = readPassengers(queue, &size);
        if (isOk) {
            if (!queue->length) printf("Passengers queue is empty!\n");
            calculateHubQueue(queue, size);
            isOk = pressEnterToContinue();
        }
    }
    printf("\nGoodbye...\n");
    free(queue);

    return 0;
}
