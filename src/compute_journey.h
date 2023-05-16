#include "a_star.h"
#include "nominatim.h"
#include <stdio.h>

struct _Journey_output_t
{
    Table_t *table;
    List_t *journey;
};
/**
 * @typedef Journey_output_t
 * @brief Compute journey output type
 *
 * @var Journey_output_t::table
 * Table of the journey graph
 * @var Journey_output_t::journey
 * List of steps of the journey
 */
typedef struct _Journey_output_t Journey_output_t;

Journey_output_t compute_journey(Table_t *table, Nominatim_t *startNomin, Nominatim_t *endNomin, char *vehicleName);
