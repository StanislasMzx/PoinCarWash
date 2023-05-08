#include "a_star.h"


double heuristic(Station_t *one_station, Station_t *end){
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
void a_star_next_stations(Table_t *one_table, char *one_station_key, double range, Heap_t **one_heap, double one_weight, Station_t *end)
{
    Station_t *one_station = table_get(one_table, one_station_key);
    assert(one_station != NULL);
    double dist_to_end = heuristic(one_station, end);
    double new_weight;
    for (int i = 0; i < one_table->length; i++)
    {
        List_t *list = one_table->slots[i];
        for (int j = 0; j < list->length; j++)
        {
            Element_t *element = &list->list[j];
            double dist = distance(element->value->coordinates, one_station->coordinates);
            if (dist > 1e-10 && dist <= range)
            {
                new_weight = one_weight+dist+heuristic(element->value, end)-dist_to_end;
                if (one_station->weight == -1 || element->value->weight > new_weight){
                    element->value->weight = new_weight;

                    // On copie les id des stations pour éviter un problème de mémoire lors du free (on va de A vers B)
                    // station A
                    char *last_station = malloc(strlen(one_station_key) + 1);
                    assert(last_station != NULL);
                    strcpy(last_station, one_station_key);
                    element->value->last_station = last_station;
                    // station B
                    char *new_key = element->key;
                    
                    heap_append(one_heap, state_create(new_key, new_weight), heap_height(*one_heap));
                }
            }
        }
    }
    return;
}


void a_star(char *id_start, char *id_end, Vehicle_t *one_vehicle, Table_t *table_station){
    Station_t *end = table_get(table_station, id_end);
    Heap_t *queue = heap_empty();
    char *new_id = id_start;
    
    
    State_t *one_state = state_create(new_id, 0);
    heap_append(&queue, one_state, heap_height(queue));
    bool again = true;
    while (again){
        one_state = heap_pop(&queue, heap_height(queue));
        if (strcmp(id_end, one_state->id_station) == 0){
            state_destroy(one_state);
            again = false;
            break;
        }
        a_star_next_stations(table_station, one_state->id_station, one_vehicle->range, &queue, one_state->weight, end);
        state_destroy(one_state);
    }
    heap_destroy(queue);
}


void print_a_star(Table_t *table_station, char *id_start, char *id_end, Vehicle_t *one_vehicle){
    a_star(id_start, id_end, one_vehicle, table_station);
    char *id = id_end;
    Station_t *one_station = table_get(table_station, id);
    while (strcmp(id, id_start) != 0){
        station_print(one_station);
        printf("id : %s\n", id);
        printf("----->\n");
        id = one_station->last_station;
        assert(id != NULL);
        one_station = table_get(table_station, id);
    }
    station_print(one_station);
}


List_t *a_star_list(Table_t *table_station, char *id_start, char *id_end, Vehicle_t *one_vehicle){
    a_star(id_start, id_end, one_vehicle, table_station);
    List_t *one_list = list_create();
    char *id = malloc(strlen(id_end)+1);
    strcpy(id, id_end);
    Station_t *one_station = station_copy(table_get(table_station, id));
    while (strcmp(id, id_start) != 0){
        list_append(one_list, id, one_station);
        id = malloc(strlen(one_station->last_station)+1);
        strcpy(id, one_station->last_station);
        assert(id != NULL);
        one_station = station_copy(table_get(table_station, id));
    }
    list_append(one_list, id, one_station);
    return one_list;
}
