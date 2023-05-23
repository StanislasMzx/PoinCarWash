#include "compute_journey.h"
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
        fprintf(stderr, "\033[31m>> Usage:\033[0m %s \033[32m\033[2m<\033[0m\033[32mstart_location\033[2m> \033[35m<\033[0m\033[35mend_location\033[2m> \033[33m<\033[0m\033[33mvehicle_name\033[2m>\033[0m\n", argv[0]);
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
