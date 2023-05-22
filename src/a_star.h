#ifndef A_STAR_H
#define A_STAR_H

#include "heap.h"
#include "vehicles.h"

#ifndef MIN
 #define MIN(x,y) (((x)<(y))? (x) : (y))
#endif // MIN

double heuristic(Station_t *one_station, Station_t *end);

int a_star_next_stations(Table_t *one_table, Station_t *one_station, char *one_station_key, double range, Heap_t **one_heap, double one_weight, Station_t *end);

int a_star(Vehicle_t *one_vehicle, Table_t *table_station, double power_min, double time_in_station_max);

void print_a_star(Table_t *table_station, List_t *one_list, Vehicle_t *one_vehicle, double power_min, double time_in_station_max);

List_t *a_star_list(Table_t *table_station, char *id_start, char *id_end, Vehicle_t *one_vehicle, double power_min, double time_in_station_max);

#endif /* A_STAR_H */
