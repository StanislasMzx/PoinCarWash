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
 * @param journey Journey
 * @param stage number of steps after departure (stage 0 is the departure)
 * @return char* 'on the road' if the vehicle is on the road else station key
 */
char *vehicle_position(Vehicle_t *vehicle, Journey_output_t *journey_list, int stage)
{
    int step = 0;
    double dist_to_next_station = 0;
    int traveled_stages = 0;
    int charging_time = 0;
    while (traveled_stages < stage)
    {
        if (step + 1 >= journey_list->journey->length)
        {
            return "on the road";
        }
        dist_to_next_station = distance(journey_list->journey->list[step].value->coordinates, journey_list->journey->list[step + 1].value->coordinates);
        step++;
        traveled_stages += ceil((dist_to_next_station / SPEED) / INTERVAL);
        if (step + 1 < journey_list->journey->length && traveled_stages <= stage)
        {
            charging_time = ceil((distance(journey_list->journey->list[step].value->coordinates, journey_list->journey->list[step + 1].value->coordinates) / vehicle->fast_charge) / INTERVAL);
            if (traveled_stages + charging_time >= stage)
            {
                return journey_list->journey->list[step].key;
            }
            traveled_stages += charging_time;
        }
    }
    return "on the road";
}

Journey_output_t *load_network(Table_t *table, char *file, int size)
{
    FILE *fp = fopen(file, "r");
    assert(fp != NULL);

    Journey_output_t *network = malloc(sizeof(Journey_output_t) * size);

    const unsigned max_line = 256;
    char line[max_line];
    int i = 0;

    fgets(line, 1024, fp);
    while (fgets(line, 1024, fp))
    {
        char departure[max_line];
        char arrival[max_line];
        char vehicle[max_line];
        if (sscanf(line, "%[^,],%[^,],%[^\n]", departure, arrival, vehicle) != 3)
        {
            fprintf(stderr, "Error: Line format is incorrect.\n");
            continue;
        }
        Nominatim_t *departure_nominatim = nominatim_fetch(departure);
        Nominatim_t *arrival_nominatim = nominatim_fetch(arrival);
        Journey_output_t journey = compute_journey(table, departure_nominatim, arrival_nominatim, vehicle);
        nominatim_destroy(departure_nominatim);
        nominatim_destroy(arrival_nominatim);
        network[i] = journey;
        i++;
    }

    fclose(fp);
    return network;
}

void network_destroy(Table_t *table, Journey_output_t *network, int size)
{
    // for (int i = 0; i < size; i++)
    // {
    //     journey_destroy(table, network[i].journey);
    // }
}
