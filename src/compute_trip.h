#include "a_star.h"
#include "nominatim.h"
#include <stdio.h>

List_t *compute_trip(Table_t *table, Nominatim_t *startNomin, Nominatim_t *endNomin, Vehicle_t *vehicle, double min_power, double time_max);
