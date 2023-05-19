#include "a_star.h"
#include <stdlib.h>
#include <math.h>

double heuristic(Station_t *one_station, Station_t *end)
{
    return distance(one_station->coordinates, end->coordinates);
}

/**
 * @brief Add the reachable station neighbors in a heap
 *
 * @param one_table hash table of stations
 * @param one_station station to get the neighbors from
 * @param range range in km
 * @param one_heap the heap
 * @param end end of A*
 * @return None
 */
int a_star_next_stations(Table_t *one_table, Station_t* one_station, char *one_station_key, double range, Heap_t **one_heap, double one_weight, Station_t *end)
{
    double dist_to_end = heuristic(one_station, end);
    double new_weight;
    bool added = 0; //if a station has been added in the heap (1 => at least one station has been added)
    for (int i = 0; i < one_table->length; i++)
    {
        List_t *list = one_table->slots[i];
        for (int j = 0; j < list->length; j++)
        {
            Element_t *element = &list->list[j];
            double dist = distance(element->value->coordinates, one_station->coordinates);
            if (dist > 1e-10 && dist <= range)
            {
                new_weight = one_weight + dist + heuristic(element->value, end) - dist_to_end;
                if (element->value->weight == -1 || element->value->weight > new_weight)
                {
                    element->value->weight = new_weight;

                    // On copie les id des stations pour éviter un problème de mémoire lors du free (on va de A vers B)
                    // station A
                    char *last_station = malloc(strlen(one_station_key) + 1);
                    assert(last_station != NULL);
                    strcpy(last_station, one_station_key);
                    free(element->value->last_station);
                    element->value->last_station = last_station;
                    // station B
                    char *new_key = element->key;

                    heap_append(one_heap, state_create(new_key, new_weight), heap_height(*one_heap));

                    added = 1;
                }
            }
        }
    }
    return added;
}

int a_star(char *id_start, char *id_end, Vehicle_t *one_vehicle, Table_t *table_station, double power_min, double time_in_station_max)
{
    Station_t *end = table_get(table_station, id_end);
    Heap_t *queue = heap_empty();
    char *new_id = id_start;

    State_t *one_state = state_create(new_id, 0);

    heap_append(&queue, one_state, heap_height(queue));

    bool again = true;
    double range_power = one_vehicle->range * (1.0 - power_min/100.0); // range if the vehicle keep always power_min% autonomy
    double range_vehicle = (double)(one_vehicle->fast_charge) * time_in_station_max/60.0; // range if the vehicle is charge at one_vehicle->fast_charge during time_in_station_max
    double range_min = MIN(range_power, range_vehicle);
    double range = range_min;

    // Whether a trip exists or not
    int tripExists = 0;

    while (again)
    {
        one_state = heap_pop(&queue, heap_height(queue));

        // No reachable station
        if (one_state == NULL){
            again = false;
            break;
        }

        // Finished trip
        if (strcmp(id_end, one_state->id_station) == 0)
        {
            state_destroy(one_state);
            again = false;
            tripExists = 1;
            break;
        }

        Station_t *one_station = table_get(table_station, one_state->id_station);
        assert(one_station != NULL);

        if (one_state->id_station != id_start)
        {
            range = MIN(range_min, (double)(one_station->power) * time_in_station_max/60.0);
        }

        a_star_next_stations(table_station, one_station, one_state->id_station, range, &queue, one_state->weight, end);

        state_destroy(one_state);
    }
    heap_destroy(queue);
    return tripExists;
}

void print_a_star(Table_t *table_station, List_t *one_list, Vehicle_t *one_vehicle, double power_min, double time_in_station_max)
{
    printf("\33[34m>> Vehicle: \33[1m%s\33[0m\n", one_vehicle->name);
    printf("            \33[34mMin. autonomy: \33[1m%.1f%%\33[0;34m\n", power_min);
    printf("            \33[34mMax. time in station: \33[1m%d min\33[0m\n", (int)time_in_station_max);

    if (list_is_empty(one_list))
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m No trip found with the given parameters.\n          Please try again with another vehicle or different parameters.\n");
        return;
    }

    int steps = one_list->length;
    Station_t *startStation = table_get(table_station, one_list->list[0].key);
    Station_t *endStation = table_get(table_station, one_list->list[steps - 1].key);

    // Start
    Station_t *prevStation;
    Station_t *currentStation = startStation;
    double distanceLeft = distance(currentStation->coordinates, endStation->coordinates);
    double distancePrev;
    int batteryBefore;
    int batteryAfter = 100;
    int chargingTime;

    printf("\33[0;32m>> Departure: \33[1m%s\33[0m\n", startStation->name);
    printf("   \33[2;32mBattery: \33[1;5m%d%%\33[0m\n", batteryAfter);
    printf("   \33[2mDistance left: \33[1m%.3f km\33[0m\n", distanceLeft);

    // Steps
    for (int i = 1; i < steps - 1; i++)
    {
        prevStation = currentStation;
        currentStation = table_get(table_station, one_list->list[i].key);
        distanceLeft = distance(currentStation->coordinates, endStation->coordinates);
        distancePrev = distance(prevStation->coordinates, currentStation->coordinates);
        batteryBefore = (int)(100.0*(1.0 - (double)distancePrev / one_vehicle->range));
        chargingTime = (int)ceil((double)(distancePrev) / (double)(one_vehicle->fast_charge) * 60.0);

        printf("\33[0;33m>> Step %d: \33[1m%s\33[0m\n", i, currentStation->name);
        printf("   \33[2;33mBattery: \33[1;5m%d%% \u2192 %d%%\33[0;2;33m  [charge for \33[1;5m%d min\33[0;2;33m]\33[0m\n", batteryBefore, batteryAfter, chargingTime);
        printf("   \33[2mDistance left: \33[1m%.3f km\33[0m\n", distanceLeft);
    }

    // End
    prevStation = currentStation;
    currentStation = endStation;
    distancePrev = distance(prevStation->coordinates, currentStation->coordinates);
    batteryBefore = (int)(100.0*(1.0 - (double)distancePrev / one_vehicle->range));

    printf("\33[0;35m>> Arrival: \33[1m%s\33[0m\n", currentStation->name);
    printf("   \33[2;35mBattery: \33[1;5m%d%%\33[0m\n", batteryBefore);

    // Print Google Maps URL
    printf("\33[0;36m>> View trip:\33[0m\nhttps://www.google.com/maps/dir/?api=1&origin=%f,%f&destination=%f,%f&waypoints=",
            startStation->coordinates->latitude, startStation->coordinates->longitude,
            endStation->coordinates->latitude, endStation->coordinates->longitude);
    for (int i = 1; i < steps - 1; i++)
    {
        Station_t *one_station = table_get(table_station, one_list->list[i].key);
        printf("%f,%f%%7C", one_station->coordinates->latitude, one_station->coordinates->longitude);
    }

    printf("\n");
}

List_t *a_star_list(Table_t *table_station, char *id_start, char *id_end, Vehicle_t *one_vehicle, double power_min, double time_in_station_max)
{
    List_t *one_list = list_create();
    if (!a_star(id_start, id_end, one_vehicle, table_station, power_min, time_in_station_max)){
        return one_list; // error
    }
    char *id = malloc(strlen(id_end) + 1);
    strcpy(id, id_end);
    Station_t *one_station = station_copy(table_get(table_station, id));
    while (strcmp(id, id_start) != 0)
    {
        list_append(one_list, id, one_station);
        id = malloc(strlen(one_station->last_station) + 1);
        strcpy(id, one_station->last_station);
        assert(id != NULL);
        one_station = station_copy(table_get(table_station, id));
    }
    list_append(one_list, id, one_station);
    List_t *one_list_reverse = list_create();
    for (int i = one_list->length - 1; i >= 0; i--)
    {
        list_append(one_list_reverse, one_list->list[i].key, one_list->list[i].value);
    }
    free(one_list->list);
    free(one_list);
    return one_list_reverse;
}
