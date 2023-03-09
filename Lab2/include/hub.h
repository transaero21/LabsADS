#ifndef HUB_H
#define HUB_H

#include "queue.h"
#include "passenger.h"

typedef struct Reception {
    Queue *queue;
    int ttl;
    int avg;
    char *op;
} Reception;

typedef struct Hub {
    Reception *rcpt;
    int size;
} Hub;

Hub *initHub(int size);
void destroyHub(Hub *hub);

void calculateHubQueue(Queue *queue, int size);
static void selectCheckIn(Hub *hub, Passenger *pax);
static void printHubStatus(Hub hub, int hr);
static int updateReceptionOnTick(Reception *rcpt);
static int updateQueueOnTick(Queue *queue, Hub *hub, int hr);

#endif //HUB_H
