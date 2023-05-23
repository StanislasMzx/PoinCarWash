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
    double min_power;
    double time_max;

    switch (argc)
    {
    case 4:
        min_power = 0.0;
        time_max = 360.0;
        break;
    case 5:
        min_power = atof(argv[4]);
        time_max = 360.0;
        break;
    case 6:
        min_power = atof(argv[4]);
        time_max = atof(argv[5]);
        break;
    default:
        fprintf(stderr, "\33[31m>> Usage:\33[0m %s \33[2;32m<\33[0;32mstart_location\33[2m> \33[35m<\33[0;35mend_location\33[2m> \33[34m<\33[0;34mvehicle_name\33[2m> \33[34m[<\33[0;34mmin_autonomy=0\33[2m>] \33[34m[<\33[0;34mmax_charging_time=360\33[2m>]\33[0m\n", argv[0]);
        return 1;
    }

    // Bad arguments
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
    if (min_power < 0.0 || min_power > 100.0)
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Invalid minimum autonomy: \"\33[31m%s\33[0m\".\n         You must enter a value between 0.0 and 100.0.\n", argv[4]);
    }
    if (time_max < 0.0)
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Invalid maximum charging time: \"\33[31m%s\33[0m\".\n         You must enter a positive value.", argv[5]);
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

    Table_t *table = load_stations(STATION_TABLE_PATH);

    // Compute trip
    List_t *trip = compute_trip(table, startNomin, endNomin, vehicle, min_power, time_max);

    print_a_star(table, trip, vehicle, min_power, time_max);

    // Free memory
    nominatim_destroy(startNomin);
    nominatim_destroy(endNomin);
    vehicle_destroy(vehicle);
    table_destroy(table);
    list_destroy(trip);

    return 0;
}
