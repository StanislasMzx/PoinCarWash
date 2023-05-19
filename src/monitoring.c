#define SPEED 100
#define INTERVAL 0.1666666667

#include "monitoring.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @brief Get the position of the vehicle at a given step after is departure
 *
 * @param vehicle Vehicle
 * @param trip Trip
 * @param stage number of steps after departure (stage 0 is the departure)
 * @return char* 'on the road' if the vehicle is on the road else station key
 */
char *vehicle_position(Vehicle_t *vehicle, Trip_output_t *trip_list, int stage)
{
    int step = 0;
    double dist_to_next_station = 0;
    int traveled_stages = 0;
    int charging_time = 0;
    while (traveled_stages < stage)
    {
        if (step + 1 >= trip_list->trip->length)
        {
            return "on the road";
        }
        dist_to_next_station = distance(trip_list->trip->list[step].value->coordinates, trip_list->trip->list[step + 1].value->coordinates);
        step++;
        traveled_stages += ceil((dist_to_next_station / SPEED) / INTERVAL);
        if (step + 1 < trip_list->trip->length && traveled_stages <= stage)
        {
            charging_time = ceil((distance(trip_list->trip->list[step].value->coordinates, trip_list->trip->list[step + 1].value->coordinates) / vehicle->fast_charge) / INTERVAL);
            if (traveled_stages + charging_time >= stage)
            {
                return trip_list->trip->list[step].key;
            }
            traveled_stages += charging_time;
        }
    }
    return "on the road";
}

Trip_output_t *load_network(Table_t *table, char *file, int size, double min_power, double time_max)
{
    FILE *fp = fopen(file, "r");
    assert(fp != NULL);

    Trip_output_t *network = malloc(sizeof(Trip_output_t) * size);

    const unsigned max_line = 256;
    char line[max_line];
    int i = 0;

    fgets(line, 1024, fp);
    while (fgets(line, 1024, fp))
    {
        char departure[max_line];
        char arrival[max_line];
        char vehicleName[max_line];
        if (sscanf(line, "%[^,],%[^,],%[^\n]", departure, arrival, vehicleName) != 3)
        {
            fprintf(stderr, "\33[31m>> Error:\33[0m Incorrect line format.\n");
            continue;
        }
        Nominatim_t *departure_nominatim = nominatim_fetch(departure);
        Nominatim_t *arrival_nominatim = nominatim_fetch(arrival);
        Vehicle_t *vehicle = vehicle_find_by_name(vehicleName);
        if (vehicle->name == NULL)
        {
            continue;
        }
        Trip_output_t trip = compute_trip(table, departure_nominatim, arrival_nominatim, vehicle, min_power, time_max);
        nominatim_destroy(departure_nominatim);
        nominatim_destroy(arrival_nominatim);
        vehicle_destroy(vehicle);
        network[i] = trip;
        i++;
    }

    fclose(fp);
    return network;
}

void network_destroy(Table_t *table, Trip_output_t *network, int size)
{
    // for (int i = 0; i < size; i++)
    // {
    //     trip_destroy(table, network[i].trip);
    // }
}
