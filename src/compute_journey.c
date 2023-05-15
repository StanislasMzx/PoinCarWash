#include "a_star.h"
#include "nominatim.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc > 5 || argc < 4)
    {
        fprintf(stderr, "Usage: %s <start_location> <end_location> <vehicle_name> <min_power=0> <time_max=10000>\n", argv[0]);
        return 1;
    }
    double min_power;
    double time_max;
    if (argc < 6){
        time_max = 10000;
    }
    if (argc < 6){
        min_power = 0;
    }
    if (argc == 6){
        time_max = atoi(argv[5]);
        min_power = atoi(argv[4]);
    }
    if (argc == 5){
        min_power = atoi(argv[4]);
    }
    Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");
    Coordinates_t *start_coordinates = malloc(sizeof(Coordinates_t)), *end_coordinates = malloc(sizeof(Coordinates_t));
    char *start_key = malloc(6), *end_key = malloc(4);
    Vehicle_t vehicle = vehicle_find_by_name(argv[3]);

    Nominatim_t *start_nomin = nominatim_fetch(argv[1]);
    start_coordinates->latitude = start_nomin->coord->latitude;
    start_coordinates->longitude = start_nomin->coord->longitude;
    Nominatim_t *end_nomin = nominatim_fetch(argv[2]);
    end_coordinates->latitude = end_nomin->coord->latitude;
    end_coordinates->longitude = end_nomin->coord->longitude;
    Station_t *start = station_create(start_nomin->name, start_coordinates, 0, 0, 0);
    Station_t *end = station_create(end_nomin->name, end_coordinates, 0, 0, 0);
    strcpy(start_key, "start");
    strcpy(end_key, "end");
    table_add(table, start_key, start);
    table_add(table, end_key, end);

    List_t *journey = a_star_list(table, "start", "end", &vehicle, min_power, time_max);
    print_a_star(table, journey, min_power, time_max);

    table_destroy(table);
    list_destroy(journey);
    nominatim_destroy(start_nomin);
    nominatim_destroy(end_nomin);

    return 0;
}
