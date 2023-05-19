#include "a_star.h"
#include <stdlib.h>

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

void print_a_star(Table_t *table_station, List_t *one_list, double power_min, double time_in_station_max)
{
    if (!list_is_empty(one_list))
    {
        int steps = one_list->length;
        Station_t *start = table_get(table_station, one_list->list[0].key);
        Station_t *end = table_get(table_station, one_list->list[steps - 1].key);
        printf("\033[0;32m>> Departure: %s\033[0m\n", start->name);
        printf("Still to go: %f km with always more than %f autonomy and waiting less than %f minutes in station\n", distance(start->coordinates, end->coordinates), power_min, time_in_station_max);
        for (int i = 1; i < one_list->length - 1; i++)
        {
            printf("\033[0;33m>> Step %d: %s\033[0m\n", i, table_get(table_station, one_list->list[i].key)->name);
            printf("Still to go: %f km\n", distance(table_get(table_station, one_list->list[i].key)->coordinates, table_get(table_station, one_list->list[steps - 1].key)->coordinates));
        }
        printf("\033[0;35m>> Arrival: %s\033[0m\n", end->name);
        printf("\033[0;36m>> View trip:\033[0m\nhttps://www.google.com/maps/dir/?api=1&origin=%f,%f&destination=%f,%f&waypoints=", start->coordinates->latitude, start->coordinates->longitude, end->coordinates->latitude, end->coordinates->longitude);
        for (int i = 1; i < one_list->length - 1; i++)
        {
            printf("%f,%f%%7C", table_get(table_station, one_list->list[i].key)->coordinates->latitude, table_get(table_station, one_list->list[i].key)->coordinates->longitude);
        }
        printf("\n");
    }
    else{
        printf("\x1b[31mError: no trip found\x1b[0m\n");
    }
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
