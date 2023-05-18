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
        fprintf(stderr, "\033[31m>> Usage:\033[0m %s \033[2;32m<\033[0;32mstart_location\033[2m> \033[35m<\033[0;35mend_location\033[2m> \033[33m<\033[0;33mvehicle_name\033[2m>\033[0m\n", argv[0]);
        return 1;
    }

    Table_t *table = load_stations(STATION_TABLE_PATH);

    Nominatim_t *startNomin = nominatim_fetch(argv[1]);

    if (startNomin == NULL)
    {
        return 1;
    }
    if (*(startNomin->name) == '\0')
    {
        fprintf(stderr, "\033[31m>> Error:\033[0m Invalid departure location: \"\033[31m%s\033[0m\".\n", argv[1]);
        // Free memory
        nominatim_destroy(startNomin);
        return 1;
    }

    Nominatim_t *endNomin = nominatim_fetch(argv[2]);
    if (endNomin == NULL)
    {
        // Free memory
        nominatim_destroy(startNomin);
        return 1;
    }
    if (*(endNomin->name) == '\0')
    {
        fprintf(stderr, "\033[31m>> Error:\033[0m Invalid arrival location: \"\033[31m%s\033[0m\".\n", argv[2]);
        // Free memory
        nominatim_destroy(startNomin);
        nominatim_destroy(endNomin);
        return 1;
    }

    Trip_output_t output = compute_trip(table, startNomin, endNomin, argv[3]);
    List_t *trip = output.trip;

    print_a_star(table, trip);

    table_destroy(table);
    list_destroy(trip);
    nominatim_destroy(startNomin);
    nominatim_destroy(endNomin);

    return 0;
}
