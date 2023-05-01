#include "heap.h"
#include "vehicles.h"


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
void next_stations(Table_t *one_table, char *one_station_key, unsigned int range, Heap_t *one_heap, double one_weight, Station_t *end)
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
            if (strcmp(element->key, one_station_key) != 0 && dist <= range)
            {
                new_weight = one_weight+dist+heuristic(element->value, end)-dist_to_end;
                if (one_station->weight == -1 || element->value->weight > new_weight)
                    element->value->weight = new_weight;

                    // On copie les id des stations pour éviter un problème de mémoire lors du free (on va de A vers B)
                    // station A
                    char *last_station = malloc(strlen(one_station_key) + 1);
                    assert(last_station != NULL);
                    strcpy(last_station, one_station_key);
                    element->value->last_station = last_station;
                    // station B
                    char *new_key = malloc(strlen(element->key) + 1);
                    assert(new_key != NULL);
                    strcpy(new_key, element->key);
                    heap_append(&one_heap, state_create(new_key, new_weight), heap_height(one_heap));
            }
        }
    }
    return;
}


void a_star(char *id_start, char *id_end, Vehicle_t *one_vehicle, Table_t *table_station){
    Station_t *end = table_get(table_station, id_end);
    Heap_t *queue = heap_empty();

    // On copie les id des stations pour éviter un problème de mémoire lors du free (on va de A vers B)
    char *new_id = malloc(strlen(id_start) + 1);
    assert(new_id != NULL);
    strcpy(new_id, id_start);
    State_t *one_state = state_create(new_id, 0);
    heap_append(&queue, one_state, heap_height(queue));
    bool again = true;
    while (again){
        one_state = heap_pop(queue, heap_height(queue));
        if (!heap_is_empty(queue) || strcasecmp(id_end, one_state->id_station)){
            again = false;
            break;
        }
        next_stations(table_station, one_state->id_station, one_vehicle->range, queue, one_state->weight, end);
    }
    heap_destroy(queue);
}


char *print_a_star(char *id_start, char *id_end, Vehicle_t *one_vehicle){
    Table_t *table_station = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");
    a_star(id_start, id_end, one_vehicle, table_station);
    char *id = id_end;
    Station_t *one_station = table_get(table_station, id);
    while (strcasecmp(id, id_start) != 0){
        station_print(one_station);
        printf("----->\n");
        id = one_station->last_station;
        one_station = table_get(table_station, id);
    }
    station_print(one_station);
    table_destroy(table_station);
}