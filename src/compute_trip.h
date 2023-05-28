#include "a_star.h"
#include "nominatim.h"
#include <stdio.h>

/**
 * @brief Compute a trip between two points
 *
 * @param table Table_t* Table of the stations
 * @param startNomin Nominatim_t* Start point
 * @param endNomin Nominatim_t* End point
 * @param vehicle Vehicle_t* Vehicle to use
 * @param min_power double Minimum autonomy throughout the trip
 * @param time_max double Maximum charging time
 * @return List_t Trip output
 */
List_t *compute_trip(Table_t *table, Nominatim_t *startNomin, Nominatim_t *endNomin, Vehicle_t *vehicle, double min_power, double time_max);
