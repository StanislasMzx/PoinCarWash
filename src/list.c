#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
    printf("%s: {name: %s, coordinates: (%f, %f), plugs_number: %d, power: %d, free: %s}", one_element->key, one_element->value->name, one_element->value->coordinates.latitude, one_element->value->coordinates.longitude, one_element->value->plugs_number, one_element->value->power, one_element->value->free ? "true" : "false");
}

/**
 * @brief Print a list
 *
 * @param one_list list to print
 */
void list_print(List_t *one_list)
{
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
