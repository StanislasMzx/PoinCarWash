#include "compute_trip.h"
#include "nominatim.h"
#include <stdio.h>

/**
 * @brief Compute a trip from the command line
 * 
 * @param argc int argument count
 * @param argv char* argument vector
 * @return int 0 if success, 1 if error
 */
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "\33[31m>> Usage:\33[0m %s \33[2;32m<\33[0;32mstart_location\33[2m> \33[35m<\33[0;35mend_location\33[2m> \33[34m<\33[0;34mvehicle_name\33[2m>\33[0m\n", argv[0]);
        return 1;
    }

    // Empty arguments
    if (*(argv[1]) == '\0')
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Empty departure location.\n");
        return 1;
    }
    if (*(argv[2]) == '\0')
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Empty arrival location.\n");
        return 1;
    }
    if (*(argv[3]) == '\0')
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Empty vehicle name.\n");
        return 1;
    }

    // Start location
    Nominatim_t *startNomin = nominatim_fetch(argv[1]);
    if (startNomin == NULL)
    {
        // Error message already printed
        return 1;
    }
    if (*(startNomin->name) == '\0')
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Invalid departure location: \"\33[31m%s\33[0m\".\n", argv[1]);
        // Free memory
        nominatim_destroy(startNomin);
        return 1;
    }

    // End location
    Nominatim_t *endNomin = nominatim_fetch(argv[2]);
    if (endNomin == NULL)
    {
        // Error message already printed
        // Free memory
        nominatim_destroy(startNomin);
        return 1;
    }
    if (*(endNomin->name) == '\0')
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Invalid arrival location: \"\33[31m%s\33[0m\".\n", argv[2]);
        // Free memory
        nominatim_destroy(startNomin);
        nominatim_destroy(endNomin);
        return 1;
    }

    // Vehicle
    Vehicle_t *vehicle = vehicle_find_by_name(argv[3]);
    if (vehicle->name == NULL)
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Vehicle not found: \"\33[31m%s\33[0m\".\n", argv[3]);
        exit(1);
    }
    printf("\33[34m>> Vehicle: \33[1m%s\33[0m\n", vehicle->name);

    Table_t *table = load_stations(STATION_TABLE_PATH);

    // Compute trip
    Trip_output_t output = compute_trip(table, startNomin, endNomin, vehicle);
    List_t *trip = output.trip;

    print_a_star(table, trip, vehicle->range);

    // Free memory
    nominatim_destroy(startNomin);
    nominatim_destroy(endNomin);
    vehicle_destroy(vehicle);
    table_destroy(table);
    list_destroy(trip);

    return 0;
}
