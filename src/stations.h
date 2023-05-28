#ifndef STATIONS_H
#define STATIONS_H

#define STATION_TABLE_PATH "../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv"

#include "table.h"

/**
 * @brief Create a hash table of stations
 *
 * @param filename
 * @return Table_t* hash table of stations
 */
Table_t *load_stations(char *filename);

/**
 * @brief Reinitialize a table (lastStation = NULL et weight = -1)
 *
 * @param one_table hash table of stations
 */
void reinitializeTable(Table_t *one_table);

/**
 * @brief Get the reachable station neighbors
 *
 * @param one_table hash table of stations
 * @param one_station station to get the neighbors from
 * @param range range in km
 * @return List_t* list of reachable station neighbors
 */
List_t *reachable_station_neighbors(Table_t *one_table, char *one_station_key, unsigned int range);

#endif /* STATIONS_H */
