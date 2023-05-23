#include "compute_trip.h"
#include "nominatim.h"
#include <stdio.h>
#include <getopt.h>

/**
 * @brief Compute a trip from the command line
 *
 * @param argc int argument count
 * @param argv char* argument vector
 * @return int 0 if success, 1 if error
 */
int main(int argc, char *argv[])
{
    int opt;
    double min_power = 0.0;
    double time_max = 360.0;

    // Parse command line arguments
    while ((opt = getopt(argc, argv, ":ha:c:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            printf("USAGE: %s [options] start_location end_location vehicle_name\n", argv[0]);
            printf("\n");
            printf("OPTIONS:\n");
            printf("  -h                      Show this help message\n");
            printf("  -a <min_autonomy>       Set the minimum autonomy of the vehicle (default: 0%%)\n");
            printf("  -c <max_charging_time>  Set the maximum charging time (default: 360min)\n");
            return 0;
        case 'a':
            min_power = atof(optarg);
            if (min_power < 0.0 || min_power > 100.0)
            {
                fprintf(stderr, "\u2514\u2500\u2500 \33[1;91mError\33[0m\n");
                fprintf(stderr, "    \u2514\u2500\u2500 Invalid minimum autonomy: \"\33[91m%s\33[0m\". You must enter a value between 0.0 and 100.0.\n", argv[4]);
                return (EXIT_FAILURE);
            }
            break;
        case 'c':
            time_max = atof(optarg);
            if (time_max < 0.0)
            {
                fprintf(stderr, "\u2514\u2500\u2500 \33[1;91mError\33[0m\n");
                fprintf(stderr, "    \u2514\u2500\u2500 Invalid maximum charging time: \"\33[91m%s\33[0m\". You must enter a positive value.\n", argv[5]);
                return (EXIT_FAILURE);
            }
            break;
        case '?':
            fprintf(stderr, "\33[31m>> Error:\33[0m Unknown option: %c\n", optopt);
            return 1;
            break;
        }
    }
    if (optind + 2 >= argc)
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Invalid number of arguments.\n");
        fprintf(stderr, "          Run `%s -h` for help.\n", argv[0]);
        return (EXIT_FAILURE);
    }

    // Start location
    Nominatim_t *startNomin = nominatim_fetch(argv[optind]);
    if (startNomin == NULL)
    {
        // Error message already printed
        return 1;
    }
    if (*(startNomin->name) == '\0')
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Invalid departure location: \"\33[31m%s\33[0m\".\n", argv[optind]);
        // Free memory
        nominatim_destroy(startNomin);
        return 1;
    }

    // End location
    Nominatim_t *endNomin = nominatim_fetch(argv[optind + 1]);
    if (endNomin == NULL)
    {
        // Error message already printed
        // Free memory
        nominatim_destroy(startNomin);
        return 1;
    }
    if (*(endNomin->name) == '\0')
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Invalid arrival location: \"\33[31m%s\33[0m\".\n", argv[optind + 1]);
        // Free memory
        nominatim_destroy(startNomin);
        nominatim_destroy(endNomin);
        return 1;
    }

    // Vehicle
    Vehicle_t *vehicle = vehicle_find_by_name(argv[optind + 2]);
    if (vehicle->name == NULL)
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Vehicle not found: \"\33[31m%s\33[0m\".\n", argv[optind + 2]);
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
