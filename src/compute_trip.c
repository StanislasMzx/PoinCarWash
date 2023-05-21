#include "compute_trip.h"
#include "a_star.h"
#include "nominatim.h"
#include <stdio.h>

/**
 * @brief Compute a trip between two points
 *
 * @param table Table_t* Table of the stations
 * @param startNomin Nominatim_t* Start point
 * @param endNomin Nominatim_t* End point
 * @param vehicle Vehicle_t* Vehicle to use
 * @param min_power double Minimum autonomy throughout the trip
 * @param time_max double Maximum charging time
 * @return Trip_output_t Trip output
 */
Trip_output_t compute_trip(Table_t *table, Nominatim_t *startNomin, Nominatim_t *endNomin, Vehicle_t *vehicle, double min_power, double time_max)
{
    Coordinates_t *startCoordinates = malloc(sizeof(Coordinates_t)), *endCoordinates = malloc(sizeof(Coordinates_t));

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
        Station_t *start = station_create(NULL, startNomin->name, startCoordinates, 0, 0, 0);
        Station_t *end = station_create(NULL, endNomin->name, endCoordinates, 0, 0, 0);
        
        table->nbStation++;
        list_append(table->slots[0], startKey, start);

        table->nbStation++;
        list_append(table->slots[1], endKey, end);
    }

    List_t *trip = a_star_list(table, "start", "end", vehicle, min_power, time_max);

    Trip_output_t output = {table, trip};

    return output;
}
