#include "nominatim.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

#include "coordinates.h"

#define API_HOST "nominatim.openstreetmap.org"
#define API_PATH "/search/?addressdetails=0&extratags=0&namedetails=0&accept-language=fr&limit=1&format=json&q="
#define API_PORT 443


/**
 * @brief Create a new Nominatim_t object
 *
 * @param name Name of the nominatim
 * @param lat Latitude of the nominatim
 * @param lon Longitude of the nominatim
 * @return Nominatim_t* Nominatim object
 */
Nominatim_t *nominatim_create(char *name, double lat, double lon)
{
    Nominatim_t *nomin = malloc(sizeof(Nominatim_t));
    nomin->name = malloc(strlen(name) + 1);
    strcpy(nomin->name, name);
    Coordinates_t *coord = malloc(sizeof(Coordinates_t));
    coord->latitude = lat;
    coord->longitude = lon;
    nomin->coord = coord;
    return nomin;
}

/**
 * @brief Destroy a Nominatim_t object
 *
 * @param nomin Nominatim object
 */
void nominatim_destroy(Nominatim_t *nomin)
{
    free(nomin->name);
    free(nomin->coord);
    free(nomin);
}

/**
 * @brief Write callback function for cURL
 *
 * @param ptr Pointer to data
 * @param size Size of each element
 * @param nmemb Number of elements
 * @param data Pointer to url_data struct
 * @return size_t Size of data
 */
size_t api_write_data(void *ptr, size_t size, size_t nmemb, API_response_t *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;

    data->size += (size * nmemb);

    tmp = realloc(data->data, data->size + 1);

    if(tmp) {
        data->data = tmp;
    } else {
        if(data->data) {
            free(data->data);
        }
        fprintf(stderr, "\033[31m>> Error:\033[0m Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}

/**
 * @brief Fetch data from the API
 *
 * @return char* API response
 */
char *api_fetch(char *query)
{
    // Initialize cURL
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (!curl) {
        return NULL;
    }

    // Escape the query string
    int queryLen = strlen(query);
    char *escapedQuery = curl_easy_escape(curl, query, queryLen);

    if (!escapedQuery) {
        fprintf(stderr, "\033[31m>> Error:\033[0m Error: Failed to escape query string.\n");
        // Free memory
        curl_easy_cleanup(curl);
        return NULL;
    }

    // Set the URL
    char url[8 + strlen(API_HOST) + strlen(API_PATH) + strlen(escapedQuery) + 1];
    sprintf(url, "https://%s%s%s", API_HOST, API_PATH, escapedQuery);
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // OSM User Agent policy
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "School project (antonin.frey@telecomnancy.eu)");
    // Set SSL options
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);
    // Set the API host and port
    curl_easy_setopt(curl, CURLOPT_PORT, API_PORT);

    // API response
    API_response_t response;
    response.size = 0;
    response.data = malloc(4096);
    if(NULL == response.data) {
        fprintf(stderr, "\033[31m>> Error:\033[0m Failed to allocate memory.\n");
        // Free memory
        curl_easy_cleanup(curl);
        curl_free(escapedQuery);
        return NULL;
    }
    response.data[0] = '\0';
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, api_write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // GET request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "\033[31m>> Error:\033[0m API fetch failed: %s\n", curl_easy_strerror(res));
        // Free memory
        free(response.data);
        curl_easy_cleanup(curl);
        curl_free(escapedQuery);
        return NULL;
    }

    // Response data
    char *data = response.data;

    // Free memory
    curl_easy_cleanup(curl);
    curl_free(escapedQuery);

    return data;
}

/**
 * @brief Parse the API response
 *
 * @param response API response
 * @return Nominatim_t* Nominatim object
 */
Nominatim_t *nominatim_parse(char *response)
{
    // Parse the JSON response
    json_object *root = json_tokener_parse(response);
    if (root == NULL) {
        fprintf(stderr, "\033[31m>> Error:\033[0m Failed to parse JSON response.\n");
        // Free memory
        free(response);
        return NULL;
    }

    // Get the first element in the array
    json_object *obj = json_object_array_get_idx(root, 0);
    if (obj == NULL) {
        // fprintf(stderr, "\033[31m>> Error:\033[0m Error: Failed to get first element in JSON array.\n");
        // Free memory
        json_object_put(root);
        return NULL;
    }

    // Extract the latitude, longitude, and display name
    json_object *lat, *lon, *name;
    if (!json_object_object_get_ex(obj, "lat", &lat) ||
        !json_object_object_get_ex(obj, "lon", &lon) ||
        !json_object_object_get_ex(obj, "display_name", &name)) {
        fprintf(stderr, "Error: Failed to extract data from JSON object.\n");
        // Free memory
        json_object_put(root);
        return NULL;
    }

    // Create a new Nominatim_t object and set its values
    char *nameStr = (char*)json_object_get_string(name);
    Nominatim_t *nomin = nominatim_create(nameStr,
                                          json_object_get_double(lat),
                                          json_object_get_double(lon));

    // Clean up and return the new object
    json_object_put(root);
    return nomin;
}

/**
 * @brief Get the nominatim of a location
 *
 * @param query Location name
 * @return Nominatim_t* Nominatim object
 */
Nominatim_t *nominatim_fetch(char *query)
{
    // Fetch data from the API
    char *response = api_fetch(query);
    if (response == NULL) {
        fprintf(stderr, "\033[31m>> Error:\033[0m Error: Failed to fetch data from API.\n");
        return NULL;
    }

    // Parse the API response
    Nominatim_t *nomin = nominatim_parse(response);
    if (nomin == NULL) {
        // fprintf(stderr, "\033[31m>> Error:\033[0m Error: Failed to parse API response.\n");
        // Free memory
        free(response);
        return NULL;
    }

    // Free memory and return the new object
    free(response);

    return nomin;
}

