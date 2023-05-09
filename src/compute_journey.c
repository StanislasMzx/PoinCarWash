#include "a_star.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <start_latitude>,<start_longitude> <end_latitude>,<start_longitude> <vehicle_name>\n", argv[0]);
        return 1;
    }

    Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");
    Coordinates_t *start_coordinates = malloc(sizeof(Coordinates_t)), *end_coordinates = malloc(sizeof(Coordinates_t));
    char *start_key = malloc(6), *end_key = malloc(4);
    Vehicle_t vehicle = vehicle_find_by_name(argv[3]);

    char *token = strsep(&argv[1], ",");
    start_coordinates->latitude = atof(token);
    token = strsep(&argv[1], ",");
    start_coordinates->longitude = atof(token);
    token = strsep(&argv[2], ",");
    end_coordinates->latitude = atof(token);
    token = strsep(&argv[2], ",");
    end_coordinates->longitude = atof(token);
    Station_t *start = station_create("Input coordinates", start_coordinates, 0, 0, 0);
    Station_t *end = station_create("Input coordinates", end_coordinates, 0, 0, 0);
    strcpy(start_key, "start");
    strcpy(end_key, "end");
    table_add(table, start_key, start);
    table_add(table, end_key, end);

    List_t *journey = a_star_list(table, "start", "end", &vehicle);
    print_a_star(table, journey);

    table_destroy(table);
    list_destroy(journey);

    return 0;
}
