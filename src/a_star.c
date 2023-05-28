#include "a_star.h"
#include <stdlib.h>
#include <math.h>

double heuristic(Station_t *one_station, Station_t *end)
{
    return distance(one_station->coordinates, end->coordinates);
}

int a_star_next_stations(Table_t *one_table, Station_t *one_station, char *one_station_key, double range, Heap_t **one_heap, double one_weight, Station_t *end)
{
    double dist_to_end = heuristic(one_station, end);
    double new_weight;
    bool added = 0; // if a station has been added in the heap (1 => at least one station has been added)
    for (int i = 2; i < one_table->length; i++)
    {
        List_t *list = one_table->slots[i];
        assert(list != NULL);
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
                    char *last_station = malloc((strlen(one_station_key) + 1) * sizeof(char));
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
    // End station
    List_t *endList = one_table->slots[1];
    assert(!list_is_empty(endList));
    Element_t *endElement = &endList->list[endList->length - 1];
    assert(endElement != NULL);
    double dist = dist_to_end;
    if (dist > 1e-10 && dist <= range)
    {
        new_weight = one_weight + dist + heuristic(endElement->value, end) - dist_to_end;
        if (endElement->value->weight == -1 || endElement->value->weight > new_weight)
        {
            endElement->value->weight = new_weight;

            // On copie les id des stations pour éviter un problème de mémoire lors du free (on va de A vers B)
            // station A
            char *last_station = malloc((strlen(one_station_key) + 1) * sizeof(char));
            assert(last_station != NULL);
            strcpy(last_station, one_station_key);
            free(endElement->value->last_station);
            endElement->value->last_station = last_station;
            // station B
            char *new_key = endElement->key;

            heap_append(one_heap, state_create(new_key, new_weight), heap_height(*one_heap));

            added = 1;
        }
    }
    return added;
}

int a_star(Vehicle_t *one_vehicle, Table_t *table_station, double power_min, double time_in_station_max)
{
    char *id_start = table_station->slots[0]->list[table_station->slots[0]->length - 1].key;
    char *id_end = table_station->slots[1]->list[table_station->slots[1]->length - 1].key;
    Station_t *end = table_station->slots[1]->list[table_station->slots[1]->length - 1].value;
    Heap_t *queue = heap_empty();

    State_t *one_state = state_create(id_start, 0);

    heap_append(&queue, one_state, heap_height(queue));

    bool again = true;
    double range_power = one_vehicle->range * (1.0 - power_min / 100.0);                    // range if the vehicle keep always power_min% autonomy
    double range_vehicle = (double)(one_vehicle->fast_charge) * time_in_station_max / 60.0; // range if the vehicle is charge at one_vehicle->fast_charge during time_in_station_max
    double range_min = MIN(range_power, range_vehicle);
    double range = range_min;

    // Whether a trip exists or not
    int tripExists = 0;

    while (again)
    {
        one_state = heap_pop(&queue, heap_height(queue));

        // No reachable station
        if (one_state == NULL)
        {
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
        Station_t *one_station = NULL;
        if (strcmp(id_start, one_state->id_station) == 0)
        {
            one_station = table_station->slots[0]->list[table_station->slots[0]->length - 1].value;
        }
        else
        {
            one_station = table_get(table_station, one_state->id_station);
        }
        assert(one_station != NULL);

        if (one_state->id_station != id_start)
        {
            range = MIN(range_min, (double)(one_station->power) * time_in_station_max / 60.0);
        }

        a_star_next_stations(table_station, one_station, one_state->id_station, range, &queue, one_state->weight, end);

        state_destroy(one_state);
    }
    heap_destroy(queue);
    return tripExists;
}

void print_a_star(Table_t *table_station, List_t *one_list, Vehicle_t *one_vehicle, double power_min, double time_in_station_max)
{
    printf("\u251c\u2500\u2500 \33[94mVehicle\33[0m\n");
    printf("\u2502\u00a0\u00a0 \33[34m\u2514\u2500\u2500 \33[1;4;94m%s\33[0m\n", one_vehicle->name);
    printf("\u2502\u00a0\u00a0     \33[34m\u251c\u2500\u2500 Min. autonomy: \33[1;94m%.1f%%\33[0m\n", power_min);
    printf("\u2502\u00a0\u00a0     \33[34m\u2514\u2500\u2500 Max. time in station: \33[1;94m%d min\33[0m\n", (int)time_in_station_max);

    if (list_is_empty(one_list))
    {
        fprintf(stderr, "\u2514\u2500\u2500 \33[1;91mError\33[0m\n");
        fprintf(stderr, "    \u2514\u2500\u2500 No trip found with the given parameters. Please try again with another vehicle or different parameters.\n");
        return;
    }

    int steps = one_list->length;
    Station_t *startStation = table_station->slots[0]->list[table_station->slots[0]->length - 1].value;
    Station_t *endStation = table_station->slots[1]->list[table_station->slots[1]->length - 1].value;

    // Start
    Station_t *prevStation;
    Station_t *currentStation = startStation;
    double distanceLeft = 0.0;
    double distancePrev;
    int batteryBefore;
    int batteryAfter = 100;
    int chargingTime;

    // Calcul real trip distance
    for (int i = 1; i < steps - 1; i++)
    {
        prevStation = currentStation;
        currentStation = table_get(table_station, one_list->list[i].key);
        distancePrev = distance(prevStation->coordinates, currentStation->coordinates);
        distanceLeft += distancePrev;
    }
    // Add distance of last step
    distanceLeft += distance(currentStation->coordinates, endStation->coordinates);
    // Reset currentStation
    currentStation = startStation;

    printf("\u251c\u2500\u2500 \33[92mDeparture\33[0m\n");
    printf("\u2502\u00a0\u00a0 \33[32m\u251c\u2500\u2500 \33[1;4;92m%s\33[0m\n", startStation->name);
    printf("\u2502\u00a0\u00a0 \33[32m\u2502\u00a0\u00a0 \u2514\u2500\u2500 Battery: \33[1;92m%d%%\33[0m\n", batteryAfter);
    printf("\u2502\u00a0\u00a0 \33[32m\u2514\u2500\u2500 Distance left: \33[1;92m%.1f km\33[0m\n", distanceLeft);

    // Steps
    for (int i = 1; i < steps - 1; i++)
    {
        prevStation = currentStation;
        currentStation = table_get(table_station, one_list->list[i].key);
        distancePrev = distance(prevStation->coordinates, currentStation->coordinates);
        distanceLeft -= distancePrev;
        batteryBefore = (int)(100.0 * (1.0 - (double)distancePrev / one_vehicle->range));
        chargingTime = (int)ceil((double)(distancePrev) / (double)(one_vehicle->fast_charge) * 60.0);

        printf("\u251c\u2500\u2500 \33[93mStep %d\33[0m\n", i);
        printf("\u2502\u00a0\u00a0 \33[33m\u251c\u2500\u2500 \33[1;4;93m%s\33[0m\n", currentStation->name);
        printf("\u2502\u00a0\u00a0 \33[33m\u2502\u00a0\u00a0 \u251c\u2500\u2500 Charge for: \33[1;93m%d min\33[0m\n", chargingTime);
        printf("\u2502\u00a0\u00a0 \33[33m\u2502\u00a0\u00a0 \u2514\u2500\u2500 Battery: \33[1;93m%d%% \33[5m\u00bb\u00bb\u00bb\33[0;1;93m %d%%\33[0m\n", batteryBefore, batteryAfter);
        printf("\u2502\u00a0\u00a0 \33[33m\u2514\u2500\u2500 Distance left: \33[1;93m%.1f km\33[0m\n", distanceLeft);
    }

    // End
    prevStation = currentStation;
    currentStation = endStation;
    distancePrev = distance(prevStation->coordinates, currentStation->coordinates);
    batteryBefore = (int)(100.0 * (1.0 - (double)distancePrev / one_vehicle->range));

    printf("\u251c\u2500\u2500 \33[95mArrival\33[0m\n");
    printf("\u2502\u00a0\u00a0 \33[35m\u2514\u2500\u2500 \33[1;4;95m%s\33[0m\n", currentStation->name);
    printf("\u2502\u00a0\u00a0     \33[35m\u2514\u2500\u2500 Battery: \33[1;95m%d%%\33[0m\n", batteryBefore);

    // Print Google Maps URL
    printf("\u2514\u2500\u2500 \33[96mView trip\33[0m\n");
    printf("    \33[36m\u2514\u2500\u2500 \33[0;1mhttps://www.google.com/maps/dir/?api=1&origin=%f,%f&destination=%f,%f&waypoints=",
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
    if (!a_star(one_vehicle, table_station, power_min, time_in_station_max))
    {
        return one_list; // error
    }
    char *id = malloc(strlen(id_end) + 1);
    strcpy(id, id_end);
    Station_t *one_station = station_copy(table_station->slots[1]->list[table_station->slots[1]->length - 1].value);
    while (strcmp(id, id_start) != 0)
    {
        list_append(one_list, id, one_station);
        id = malloc(strlen(one_station->last_station) + 1);
        strcpy(id, one_station->last_station);
        assert(id != NULL);
        if (strcmp(id, id_start) == 0)
        {
            one_station = station_copy(table_station->slots[0]->list[table_station->slots[0]->length - 1].value);
            break;
        }
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
