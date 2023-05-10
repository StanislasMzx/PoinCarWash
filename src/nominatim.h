#ifndef NOMINATIM_H
#define NOMINATIM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "coordinates.h"

struct _Nominatim_t
{
    char *name;
    Coordinates_t *coord;
};
/**
 * @typedef Nominatim_t
 * @brief Nominatim type
 *
 * @var Nominatim_t::name
 * Name of the nominatim
 * @var Nominatim_t::coord
 * Coordinates of the nominatim
 */
typedef struct _Nominatim_t Nominatim_t;

Nominatim_t *create_nominatim(char *name, double lat, double lon);

void destroy_nominatim(Nominatim_t *nomin);

char *fetch_api(char *query);

Nominatim_t *parse_nominatim(char *response);

Nominatim_t *get_nominatim(char *query);

#endif /* NOMINATIM_H */
