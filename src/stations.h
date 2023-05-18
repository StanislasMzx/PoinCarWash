#ifndef STATIONS_H
#define STATIONS_H

#define STATION_TABLE_PATH "../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv"

#include "table.h"

Table_t *load_stations(char *filename);

void table_destroy_after_stations_load(Table_t *one_table);

List_t *reachable_station_neighbors(Table_t *one_table, char *one_station_key, unsigned int range);

#endif /* STATIONS_H */
