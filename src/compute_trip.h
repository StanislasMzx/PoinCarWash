#include "a_star.h"
#include "nominatim.h"
#include <stdio.h>

struct _Trip_output_t
{
    Table_t *table;
    List_t *trip;
};
/**
 * @typedef Trip_output_t
 * @brief Compute trip output type
 *
 * @var Trip_output_t::table
 * Table of the trip graph
 * @var Trip_output_t::trip
 * List of steps of the trip
 */
typedef struct _Trip_output_t Trip_output_t;

Trip_output_t compute_trip(Table_t *table, Nominatim_t *startNomin, Nominatim_t *endNomin, char *vehicleName);
