#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

/**
 * @brief Create a List_t object
 *
 * @return List_t*
 */
List_t *list_create()
{
    List_t *one_list = malloc(sizeof(List_t));
    one_list->length = 0;
    one_list->capacity = 2;
    one_list->list = malloc(one_list->capacity * sizeof(Element_t));
    return one_list;
}

/**
 * @brief Destroy the List_t object
 *
 * @param one_list list to destroy
 */
void list_destroy(List_t *one_list)
{
    for (int j = 0; j < one_list->length; j++)
    {
        free(one_list->list[j].key);
        free(one_list->list[j].value->name);
        free(one_list->list[j].value->coordinates);
        free(one_list->list[j].value->last_station);
        free(one_list->list[j].value);
    }
    free(one_list->list);
    free(one_list);
}

/**
 * @brief Check if the list is empty
 *
 * @param one_list list to check
 * @return bool true if the list is empty, false otherwise
 */
bool list_is_empty(List_t *one_list)
{
    return one_list->length == 0;
}

/**
 * @brief Append an element to the list
 *
 * @param one_list list to append to
 * @param one_key key of the element
 * @param one_station value of the element
 */
void list_append(List_t *one_list, char *one_key, Station_t *one_station)
{
    if (one_list->length == one_list->capacity)
    {
        one_list->capacity *= 2;
        one_list->list = realloc(one_list->list, one_list->capacity * sizeof(Element_t));
    }
    Element_t one_element = {one_key, one_station};
    one_list->list[one_list->length] = one_element;
    one_list->length++;
}

/**
 * @brief Print an element
 *
 * @param one_element element to print
 */
void element_print(Element_t *one_element)
{
    assert(one_element != NULL);
    assert(one_element->value != NULL);
    assert(one_element->value->coordinates != NULL);
    printf("%s: id: %d{name: %s, coordinates: (%f, %f), plugs_number: %d, power: %d, free: %s, last station: %s}\n", one_element->key, one_element->value->id, one_element->value->name, one_element->value->coordinates->longitude, one_element->value->coordinates->latitude, one_element->value->plugs_number, one_element->value->power, one_element->value->free ? "true" : "false", one_element->value->last_station);
}

/**
 * @brief Print an station
 *
 * @param one_station station to print
 */
void station_print(Station_t *one_station)
{
    if (one_station == NULL)
    {
        printf("Pointeur vide");
        return;
    }
    printf("\n\nid:%d{name: %s, coordinates: (%f, %f), plugs_number: %d, power: %d, free: %s, last station: %s}\n", one_station->id, one_station->name, one_station->coordinates->longitude, one_station->coordinates->latitude, one_station->plugs_number, one_station->power, one_station->free ? "true" : "false", one_station->last_station);
}

/**
 * @brief Print a list
 *
 * @param one_list list to print
 */
void list_print(List_t *one_list)
{
    printf("Len : %d, ", one_list->length);
    for (int i = 0; i < one_list->length; i++)
    {
        element_print(&one_list->list[i]);
    }
}

/**
 * @brief Check if the list contains an element
 *
 * @param one_list list to check
 * @param one_key key of the element
 * @return bool true if the list contains the element, false otherwise
 */
bool list_contains(List_t *one_list, char *one_key)
{
    for (int i = 0; i < one_list->length; i++)
    {
        if (strcmp(one_list->list[i].key, one_key) == 0)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Find an element in the list
 *
 * @param one_list list to search in
 * @param one_key key of the element
 * @return Station_t* value of the element if found, NULL otherwise
 */
Station_t *list_find(List_t *one_list, char *one_key)
{
    for (int i = 0; i < one_list->length; i++)
    {
        if (strcmp(one_list->list[i].key, one_key) == 0)
        {
            return one_list->list[i].value;
        }
    }
    return NULL;
}

Station_t *station_create(int id, char *name, Coordinates_t *coordinates, int plugs_number, int power, bool free)
{
    Station_t *station = malloc(sizeof(Station_t));
    assert(station != NULL);

    station->name = malloc(strlen(name) + 1);
    strcpy(station->name, name);

    station->id = id;
    station->coordinates = coordinates;
    station->plugs_number = plugs_number;
    station->power = power;
    station->free = free;
    station->weight = -1; // -1 is like inf here
    station->last_station = NULL;

    return station;
}

Station_t *station_copy(Station_t *one_station)
{
    assert(one_station != NULL);
    Coordinates_t *copy_coord = malloc(sizeof(Coordinates_t));
    copy_coord->latitude = one_station->coordinates->latitude;
    copy_coord->longitude = one_station->coordinates->longitude;
    char *copy_last_station;
    if (one_station->last_station == NULL)
    {
        copy_last_station = NULL;
    }
    else
    {
        copy_last_station = malloc(strlen(one_station->last_station) + 1);
        strcpy(copy_last_station, one_station->last_station);
    }
    Station_t *copy = station_create(one_station->id, one_station->name, copy_coord, one_station->plugs_number, one_station->power, one_station->free);
    copy->last_station = copy_last_station;
    copy->weight = one_station->weight;
    return copy;
}

/**
 * @brief Calculate the travel time between two stations in ticks
 *
 * @param one_station first station
 * @param other_station second station
 * @return int travel time in ticks
 */
int travel_ticks(Station_t *one_station, Station_t *other_station)
{
    double dist = distance(one_station->coordinates, other_station->coordinates);
    double travelTime = dist / VEHICLE_SPEED * 60.0;
    int travelTicks = (int)ceil(travelTime / 10.0);

    return travelTicks;
}
