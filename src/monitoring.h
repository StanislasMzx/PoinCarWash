#ifndef MONITORING_H
#define MONITORING_H

#include "compute_trip.h"

char *vehicle_position(Vehicle_t *vehicle, Trip_output_t *trip, int stage);

Trip_output_t *load_network(Table_t *table, char *file, int size);

void network_destroy(Table_t *table, Trip_output_t *network, int size);

#endif /* MONITORING_H */
