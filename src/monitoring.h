#ifndef MONITORING_H
#define MONITORING_H

#include "compute_journey.h"

char *vehicle_position(Vehicle_t *vehicle, Journey_output_t *journey, int stage);

Journey_output_t *load_network(Table_t *table, char *file, int size);

void network_destroy(Table_t *table, Journey_output_t *network, int size);

#endif /* MONITORING_H */
