#include <stdio.h>
#include <stdlib.h>

#include "include/hub.h"
#include "include/queue.h"
#include "include/passenger.h"
#include "include/utils.h"

Hub *initHub(int size) {
    Hub *hub = calloc(1, sizeof(Hub));
    hub->rcpt = calloc(size, sizeof(Reception));
    for (int i = 0; i < size; i++) {
        hub->rcpt[i].queue = calloc(1, sizeof(Queue));
        hub->rcpt[i].op = calloc(1, sizeof(Queue));
    }
    hub->size = size;
    return hub;
}

void destroyHub(Hub *hub) {
    for (int i = 0; i < hub->size; ++i) {
        free(hub->rcpt[i].queue);
        free(hub->rcpt[i].op);
    }
    free(hub->rcpt);
    free(hub);
}

void calculateHubQueue(Queue *queue, int size) {
    Hub *hub = initHub(size);

    int isOk = 1, updated;
    for (int hr = 1; isOk; hr++) {
        updated = 0, isOk = !!queue->length;

        updated |= updateQueueOnTick(queue, hub, hr);

        for (int i = 0; i < hub->size; i++) {
            updated |= updateReceptionOnTick(&hub->rcpt[i]);
            isOk |= !!hub->rcpt[i].queue->length;
        }

        if (updated) printHubStatus(*hub, hr);
    }

    destroyHub(hub);
}

static int updateReceptionOnTick(Reception *rcpt) {
    int updated = 0;

    if (rcpt->queue->length) {
        Passenger *pax = get(rcpt->queue);
        pax->ts--;
        if (!pax->ts) {
            freePassenger(pop(rcpt->queue));
            dropStringBeforeChar(&rcpt->op, ' ');
            updated = 1;
        }
    }

    return updated;
}

static int updateQueueOnTick(Queue *queue, Hub *hub, int hr) {
    int updated = 0;

    for (int i = 0; i < queue->length; i++) {
        Passenger *pax = get(queue);
        if (pax->ta <= hr) {
            selectCheckIn(hub, pax);
            pop(queue);
            updated = 1;
            i--;
        } else break;
    }

    return updated;
}

static void selectCheckIn(Hub *hub, Passenger *pax) {
    Reception *slct = &hub->rcpt[0], *curr;

    for (int i = 1; i < hub->size; i++) {
        curr = &hub->rcpt[i];
        if (slct->queue->length > curr->queue->length ||
            slct->queue->length == curr->queue->length &&
            slct->avg > curr->avg) slct = curr;
    }

    slct->avg = (slct->avg * slct->ttl + pax->ts) / ++slct->ttl;
    if (!slct->queue->length) pax->ts++;
    push(slct->queue, pax);
    appendString(&slct->op, pax->id);
}

static void printHubStatus(Hub hub, int hr) {
    printf("State of queues at check-ins at %d:\n", hr);
    for (int i = 0; i < hub.size; i++) {
        printf("%d.\t=> %s", i + 1, hub.rcpt[i].op);
        if (!hub.rcpt[i].queue->length) printf("Check-in is empty!");
        printf("\n");
    }
    printf("\n");
}
