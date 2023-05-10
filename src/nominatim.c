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
Nominatim_t *create_nominatim(char *name, double lat, double lon)
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
void destroy_nominatim(Nominatim_t *nomin)
{
    free(nomin->name);
    free(nomin->coord);
    free(nomin);
}

/**
 * @brief Callback function for the API response
 *
 * @param ptr Pointer to the data
 * @param size Size of each element
 * @param nmemb Number of elements
 * @param userdata Pointer to the userdata
 * @return size_t Size of the data
 */
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t realsize = size * nmemb;
    char *temp = realloc(userdata, strlen(userdata) + realsize + 1);
    if (temp == NULL) {
        free(userdata);
        return 0;
    }
    userdata = temp;
    strncat(userdata, ptr, realsize);
    return realsize;
}

/**
 * @brief Fetch data from the API
 *
 * @return char* API response
 */
char *fetch_api(char *query)
{
    // Initialize cURL
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (!curl) {
        return NULL;
    }

    // Set the URL
    char *url = malloc(strlen(API_PATH) + strlen(query) + 1);
    sprintf(url, "%s%s", API_PATH, curl_easy_escape(curl, query, 0));
    curl_easy_setopt(curl, CURLOPT_URL, url);
    free(url);

    // Set SSL options
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);

    // Set the API host and port
    curl_easy_setopt(curl, CURLOPT_PORT, API_PORT);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, "Host: "API_HOST);

    // Set the query string
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

    // Set the callback function for the API response
    char *response = malloc(1);
    response[0] = '\0';
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    // GET request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    // Free memory
    curl_easy_cleanup(curl);
    return response;
}

/**
 * @brief Parse the API response
 *
 * @param response API response
 * @return Nominatim_t* Nominatim object
 */
Nominatim_t *parse_nominatim(char *response)
{
    // Parse the JSON response
    json_object *root = json_tokener_parse(response);
    if (root == NULL) {
        fprintf(stderr, "Error: Failed to parse JSON response.\n");
        return NULL;
    }

    // Get the first element in the array
    json_object *obj = json_object_array_get_idx(root, 0);
    if (obj == NULL) {
        fprintf(stderr, "Error: Failed to get first element in JSON array.\n");
        return NULL;
    }

    // Extract the latitude, longitude, and display name
    json_object *lat, *lon, *name;
    if (!json_object_object_get_ex(obj, "lat", &lat) ||
        !json_object_object_get_ex(obj, "lon", &lon) ||
        !json_object_object_get_ex(obj, "display_name", &name)) {
        fprintf(stderr, "Error: Failed to extract data from JSON object.\n");
        return NULL;
    }

    // Create a new Nominatim_t object and set its values
    Nominatim_t *nomin = create_nominatim(json_object_get_string(name),
                                          json_object_get_double(lat),
                                          json_object_get_double(lon));

    // Clean up and return the new object
    json_object_put(root);
    return nomin;
}

Nominatim_t *get_nominatim(char *query)
{
    // ...
    // return nomin
}



// function fetchLocation(location: string): Coordinates {
//   const endpoint = process.env.NOMINATIM_API_ENDPOINT as string;
//   const queryParams = new URLSearchParams({
//     q: location,
//     format: "json",
//   });

//   // Fetch location from API
//   const fetchURL = `${endpoint}?${queryParams.toString()}`;
//   const response = fetch(fetchURL, {
//     headers: {
//       "User-Agent": "School project (antonin.frey@telecomnancy.eu)"
//     },
//   });   
//   const data = response.json();

//   // Parse OSM URL
//   try {
//     const res = data[0];
//     const lat = res.lat;
//     const lon = res.lon;
//     const displayName = res.displayName;

//     return {
//       displayName,
//       lat,
//       lon
//     };
//   } catch (error) {
//     throw error;
//   }
// }
