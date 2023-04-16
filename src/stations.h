#ifndef STATIONS_H
#define STATIONS_H

#include "table.h"

Station_t *station_create(char *name, Coordinates_t coordinates, int plugs_number, int power, bool free);

Table_t *load_stations(char *filename);

void table_destroy_after_stations_load(Table_t *one_table);

List_t *reachable_station_neighbors(Table_t *one_table, char *one_station_key, unsigned int range);

#endif /* STATIONS_H */
