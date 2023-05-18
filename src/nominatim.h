#ifndef NOMINATIM_H
#define NOMINATIM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "coordinates.h"

struct _API_response_t
{
    size_t size;
    char *data;
};
/**
 * @typedef API_response_t
 * @brief API response type
 *
 * @var API_response_t::size
 * Size of the response
 * @var API_response_t::data
 * Response data
 */
typedef struct _API_response_t API_response_t;

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

Nominatim_t *nominatim_create(char *name, double lat, double lon);

void nominatim_destroy(Nominatim_t *nomin);

size_t api_write_data(void *contents, size_t size, size_t nmemb, void *userp);

char *api_fetch(char *query);

Nominatim_t *nominatim_parse(char *response);

Nominatim_t *nominatim_fetch(char *query);

#endif /* NOMINATIM_H */
