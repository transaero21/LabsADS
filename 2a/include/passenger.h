#ifndef PASSENGER_H
#define PASSENGER_H

#include "queue.h"

typedef struct Passenger {
    char *id;
    int ta;
    int ts;
} Passenger;

Passenger *initPassenger();
void freePassenger(Passenger *pax);

int readPassengers(Queue *queue, int *size);
static int stringToPassenger(const char *string, Passenger *pax);

#endif //PASSENGER_H
