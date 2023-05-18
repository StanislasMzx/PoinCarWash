#include "nominatim.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "coordinates.h"

#define API_HOST "nominatim.openstreetmap.org"
#define API_PATH "/search/?addressdetails=0&extratags=0&namedetails=0&accept-language=fr&limit=1&format=json&q="
#define API_PORT 443
#define API_USER_AGENT "School project (antonin.frey@telecomnancy.eu)"


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
 * @param contents Void pointer to the data
 * @param size Size of each element
 * @param nmemb Number of elements
 * @param userp Void pointer to the user data
 * @return size_t Size of the data
 */
size_t api_write_data(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    API_response_t *mem = (API_response_t *)userp;
 
    char *ptr = realloc(mem->data, mem->size + realsize + 1);
    if (!ptr) {
        fprintf(stderr, "\33[31m>> Error:\33[0m Failed to allocate memory.\n");
        return 0;
    }
    
    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;
    
    return realsize;
}

/**
 * @brief Fetch data from the API
 *
 * @return char* API response, empty if query error and NULL if API error
 */
char *api_fetch(char *query)
{
    // Initialize cURL
    CURLcode res;
    CURL *curl;
    API_response_t response;
    response.size = 0;
    response.data = malloc(1);
    if (response.data == NULL) {
        fprintf(stderr, "\33[31m>> Error:\33[0m Failed to allocate memory.\n");
        return NULL;
    }
    
    // Start cURL session
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "\33[31m>> Error:\33[0m Failed to initialize cURL.\n");
        // Free memory
        free(response.data);
        curl_global_cleanup();
        return NULL;
    }

    // Escape the query string
    size_t queryLen = strlen(query);
    char *escapedQuery = curl_easy_escape(curl, query, queryLen);
    assert(escapedQuery != NULL);

    // Set the URL
    size_t url_length = snprintf(NULL, 0, "https://%s%s%s", API_HOST, API_PATH, escapedQuery);
    char* url = (char*)malloc((url_length + 1) * sizeof(char));
    snprintf(url, url_length + 1, "https://%s%s%s", API_HOST, API_PATH, escapedQuery);
    assert(url != NULL);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    free(url);

    // OSM User Agent policy
    curl_easy_setopt(curl, CURLOPT_USERAGENT, API_USER_AGENT);

    // API response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, api_write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

    // GET request
    res = curl_easy_perform(curl);
    char *data = malloc(response.size + 1);

    if (res != CURLE_OK) {
        fprintf(stderr, "\33[31m>> Error:\33[0m API fetch failed: \33[4;91m%s\33[0m\n", curl_easy_strerror(res));
        free(data);
        data = NULL;
    } else if (data == NULL) {
        fprintf(stderr, "\33[31m>> Error:\33[0m Failed to allocate memory.\n");
    } else {
        strcpy(data, response.data);
    }

    // Free memory
    free(response.data);
    curl_free(escapedQuery);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return data;
}

/**
 * @brief Parse the API response
 *
 * @param response API response
 * @return Nominatim_t* Nominatim object, NULL if error
 */
Nominatim_t *nominatim_parse(char *response)
{
    // Initialize values
    double lat;
    double lon;
    char *displayName = NULL;

    // Values to fill
    const char *latStart = strstr(response, "\"lat\":");
    const char *lonStart = strstr(response, "\"lon\":");
    const char *displayNameStart = strstr(response, "\"display_name\":");

    // Check if valid response
    if (!latStart || !lonStart || !displayNameStart) {
        return NULL;
    }

    const char *displayNameEnd = strstr(displayNameStart, "\",");

    // Check if valid response
    if (!displayNameEnd) {
        return NULL;
    }

    // Skip atrribute names
    latStart += 7;
    lonStart += 7;
    displayNameStart += 16;

    // Extract coordinates
    lat = strtod(latStart, NULL);
    lon = strtod(lonStart, NULL);

    // Extract display name
    size_t length = displayNameEnd - displayNameStart;
    displayName = malloc((length + 1) * sizeof(char));
    strncpy(displayName, displayNameStart, length);
    displayName[length] = '\0';

    // Create nominatim object
    Nominatim_t *nomin = nominatim_create(displayName, lat, lon);

    // Free memory
    free(displayName);

    return nomin;
}

/**
 * @brief Get the nominatim of a location
 *
 * @param query Location name
 * @return Nominatim_t* Nominatim object, empty name if query error and NULL if API error
 */
Nominatim_t *nominatim_fetch(char *query)
{
    // Fetch data from the API
    char *response = api_fetch(query);
    if (response == NULL) {
        fprintf(stderr, "          Please check your internet connection and that \33[36m%s\33[0m is reachable.\n", API_HOST);
        return NULL;
    }

    Nominatim_t *nomin;

    if (*response == '\0') {
        nomin = nominatim_create("", 0, 0);
    } else {
        // Parse the API response
        nomin = nominatim_parse(response);
        if (nomin == NULL) {
            nomin = nominatim_create("", 0, 0);
        }
    }

    // Free memory
    free(response);

    return nomin;
}

