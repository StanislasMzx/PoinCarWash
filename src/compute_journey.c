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
Journey_output_t compute_journey(Nominatim_t *startNomin, Nominatim_t *endNomin, char *vehicleName)
{
    Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");
    Coordinates_t *startCoordinates = malloc(sizeof(Coordinates_t)), *endCoordinates = malloc(sizeof(Coordinates_t));
    char *startKey = malloc(6), *endKey = malloc(4);
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

    Station_t *start = station_create(startNomin->name, startCoordinates, 0, 0, 0);
    Station_t *end = station_create(endNomin->name, endCoordinates, 0, 0, 0);
    strcpy(startKey, "start");
    strcpy(endKey, "end");
    table_add(table, startKey, start);
    table_add(table, endKey, end);

    List_t *journey = a_star_list(table, "start", "end", &vehicle);

    Journey_output_t output = {table, journey};

    return output;
}

/**
 * @brief Compute the journey from the command line
 * 
 * @param argc int argument count
 * @param argv char* argument vector
 * @return int 0 if success, 1 if error
*/
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <start_location> <end_location> <vehicle_name>\n", argv[0]);
        return 1;
    }

    Nominatim_t *startNomin = nominatim_fetch(argv[1]);
    Nominatim_t *endNomin = nominatim_fetch(argv[2]);

    if (startNomin == NULL)
    {
        fprintf(stderr, "\033[31m>> Error:\033[0m Invalid departure location\n");
        return 1;
    }
    if (endNomin == NULL)
    {
        fprintf(stderr, "\033[31m>> Error:\033[0m Invalid arrival location\n");
        // Free memory
        nominatim_destroy(startNomin);
        return 1;
    }

    Journey_output_t output = compute_journey(startNomin, endNomin, argv[3]);
    Table_t *table = output.table;
    List_t *journey = output.journey;

    print_a_star(table, journey);

    table_destroy(table);
    list_destroy(journey);
    nominatim_destroy(startNomin);
    nominatim_destroy(endNomin);

    return 0;
}
