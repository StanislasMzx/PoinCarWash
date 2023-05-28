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

/**
 * @brief Create a new Nominatim_t object
 *
 * @param name Name of the nominatim
 * @param lat Latitude of the nominatim
 * @param lon Longitude of the nominatim
 * @return Nominatim_t* Nominatim object
 */
Nominatim_t *nominatim_create(char *name, double lat, double lon);

/**
 * @brief Destroy a Nominatim_t object
 *
 * @param nomin Nominatim object
 */
void nominatim_destroy(Nominatim_t *nomin);

/**
 * @brief Write callback function for cURL
 *
 * @param contents Void pointer to the data
 * @param size Size of each element
 * @param nmemb Number of elements
 * @param userp Void pointer to the user data
 * @return size_t Size of the data
 */
size_t api_write_data(void *contents, size_t size, size_t nmemb, void *userp);

/**
 * @brief Fetch data from the API
 *
 * @return char* API response, empty if query error and NULL if API error
 */
char *api_fetch(char *query);

/**
 * @brief Parse the API response
 *
 * @param response API response
 * @return Nominatim_t* Nominatim object, NULL if error
 */
Nominatim_t *nominatim_parse(char *response);

/**
 * @brief Get the nominatim of a location
 *
 * @param query Location name
 * @return Nominatim_t* Nominatim object, empty name if query error and NULL if API error
 */
Nominatim_t *nominatim_fetch(char *query);

#endif /* NOMINATIM_H */
