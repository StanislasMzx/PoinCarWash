#ifndef A_STAR_H
#define A_STAR_H

#include "heap.h"
#include "vehicles.h"

#ifndef MIN
 #define MIN(x,y) (((x)<(y))? (x) : (y))
#endif // MIN

/**
 * @brief Calculate the heuristic distance from one station to the end station
 *
 * @param one_station Station to calculate the heuristic distance
 * @param end End station
 * @return double Heuristic distance
 */
double heuristic(Station_t *one_station, Station_t *end);

/**
 * @brief Add the reachable station neighbors in a heap
 *
 * @param one_table hash table of stations
 * @param one_station station to get the neighbors from
 * @param range range in km
 * @param one_heap the heap
 * @param end end of A*
 * @return 1 if at least one station was added, 0 otherwise
 */
int a_star_next_stations(Table_t *one_table, Station_t *one_station, char *one_station_key, double range, Heap_t **one_heap, double one_weight, Station_t *end);

/**
 * @brief A* algorithm
 *
 * @param one_vehicle vehicle to use
 * @param table_station hash table of stations
 * @param power_min minimum power of the vehicle
 * @param time_in_station_max maximum time in a station
 * @return 1 if a trip was found, 0 otherwise
 */
int a_star(Vehicle_t *one_vehicle, Table_t *table_station, double power_min, double time_in_station_max);

/**
 * @brief Print the trip found by A*
 *
 * @param table_station hash table of stations
 * @param one_list list of stations
 * @param one_vehicle vehicle used
 * @param power_min minimum power of the vehicle
 * @param time_in_station_max maximum time in a station
 */
void print_a_star(Table_t *table_station, List_t *one_list, Vehicle_t *one_vehicle, double power_min, double time_in_station_max);

/**
 * @brief Execute A* and return the list of visited stations
 * 
 * @param table_station hash table of stations
 * @param id_start id of the start station
 * @param id_end id of the end station
 * @param one_vehicle vehicle used
 * @param power_min minimum power of the vehicle
 * @param time_in_station_max maximum time in a station
 * @return List_t* list of visited stations
 */
List_t *a_star_list(Table_t *table_station, char *id_start, char *id_end, Vehicle_t *one_vehicle, double power_min, double time_in_station_max);

#endif /* A_STAR_H */
