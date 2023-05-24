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
 * @return List_t Trip output
 */
List_t *compute_trip(Table_t *table, Nominatim_t *startNomin, Nominatim_t *endNomin, Vehicle_t *vehicle, double min_power, double time_max)
{
    Coordinates_t *startCoordinates = malloc(sizeof(Coordinates_t)), *endCoordinates = malloc(sizeof(Coordinates_t));

    startCoordinates->latitude = startNomin->coord->latitude;
    startCoordinates->longitude = startNomin->coord->longitude;
    endCoordinates->latitude = endNomin->coord->latitude;
    endCoordinates->longitude = endNomin->coord->longitude;

    char *startKey = malloc(6), *endKey = malloc(4);
    strcpy(startKey, "start");
    strcpy(endKey, "end");
    Station_t *start = station_create(-1, startNomin->name, startCoordinates, 0, 0, 0);
    Station_t *end = station_create(-1, endNomin->name, endCoordinates, 0, 0, 0);
    
    reinitializeTable(table);

    list_append(table->slots[0], startKey, start);

    list_append(table->slots[1], endKey, end);

    List_t *trip = a_star_list(table, "start", "end", vehicle, min_power, time_max);


    return trip;
}
