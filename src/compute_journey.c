#include "compute_journey.h"
#include "a_star.h"
#include "nominatim.h"
#include <stdio.h>

/**
 * @brief Compute the journey between two points
 *
 * @param startNomin Nominatim_t* Start point
 * @param endNomin Nominatim_t* End point
 * @param vehicleName char* Name of the vehicle
 * @return Journey_output_t Journey output
 */
Journey_output_t compute_journey(Table_t *table, Nominatim_t *startNomin, Nominatim_t *endNomin, char *vehicleName)
{
    // Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");
    Coordinates_t *startCoordinates = malloc(sizeof(Coordinates_t)), *endCoordinates = malloc(sizeof(Coordinates_t));
    Vehicle_t vehicle = vehicle_find_by_name(vehicleName);

    if (vehicle.name == NULL)
    {
        fprintf(stderr, "\033[31m>> Error:\033[0m Vehicle not found.\n");
        exit(1);
    }

    startCoordinates->latitude = startNomin->coord->latitude;
    startCoordinates->longitude = startNomin->coord->longitude;
    endCoordinates->latitude = endNomin->coord->latitude;
    endCoordinates->longitude = endNomin->coord->longitude;

    if (table_contains(table, "start") && table_contains(table, "end"))
    {
        free(table_get(table, "start")->coordinates);
        table_get(table, "start")->coordinates = startCoordinates;
        free(table_get(table, "end")->coordinates);
        table_get(table, "end")->coordinates = endCoordinates;
    }
    else
    {
        char *startKey = malloc(6), *endKey = malloc(4);
        strcpy(startKey, "start");
        strcpy(endKey, "end");
        Station_t *start = station_create(startNomin->name, startCoordinates, 0, 0, 0);
        Station_t *end = station_create(endNomin->name, endCoordinates, 0, 0, 0);
        table_add(table, startKey, start);
        table_add(table, endKey, end);
    }

    List_t *journey = a_star_list(table, "start", "end", &vehicle);

    Journey_output_t output = {table, journey};

    return output;
}
