#ifndef A_STAR_H
#define A_STAR_H

#include "heap.h"
#include "vehicles.h"

double heuristic(Station_t *one_station, Station_t *end);

void a_star_next_stations(Table_t *one_table, char *one_station_key, double range, Heap_t **one_heap, double one_weight, Station_t *end);

void a_star(char *id_start, char *id_end, Vehicle_t *one_vehicle, Table_t *table_station, double power_min);

void print_a_star(Table_t *table_station, List_t *one_list, double power_min);

List_t *a_star_list(Table_t *table_station, char *id_start, char *id_end, Vehicle_t *one_vehicle, double power_min);

#endif /* A_STAR_H */
