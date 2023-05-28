#include <stdbool.h>
#include "coordinates.h"

#ifndef __LIST_H__
#define __LIST_H__

#define VEHICLE_SPEED 100

struct _Station_t
{
    int id;
    char *name;
    Coordinates_t *coordinates;
    int plugs_number;
    int power;
    bool free;
    double weight;
    char *last_station;
};
/**
 * @typedef Station_t
 * @brief Station type
 *
 * @var Station::name
 * Name of the station
 * @var Station::coordinates
 * Coordinates of the station
 * @var Station::plugs_number
 * Number of plugs of the station
 * @var Station::fast_charge
 * Fast charge availability of the station
 * @var Station::power
 * Power of the station in kW
 * @var Station::free
 * Free availability of the station
 * @var Station::weight
 * Best weight in A* for this station
 * @var Station::last_station
 * Id of the station used to go in that one with A*
 */
typedef struct _Station_t Station_t;

struct _Element_t
{
    char *key;
    Station_t *value;
};
/**
 * @typedef Element_t
 * @brief Element type
 *
 * @var Element_t::key
 * Key of the element
 * @var Element_t::value
 * Value of the element
 */
typedef struct _Element_t Element_t;

struct _List_t
{
    Element_t *list;
    int length;
    int capacity;
};
/**
 * @typedef List_t
 * @brief List type
 *
 * @var List_t::list
 * List of elements
 * @var List_t::length
 * Length of the list
 * @var List_t::capacity
 * Capacity of the list
 */
typedef struct _List_t List_t;

/**
 * @brief Create a List_t object
 *
 * @return List_t*
 */
List_t *list_create();

/**
 * @brief Destroy the List_t object
 *
 * @param one_list list to destroy
 */
void list_destroy(List_t *one_list);

/**
 * @brief Check if the list is empty
 *
 * @param one_list list to check
 * @return bool true if the list is empty, false otherwise
 */
bool list_is_empty(List_t *one_list);

/**
 * @brief Append an element to the list
 *
 * @param one_list list to append to
 * @param one_key key of the element
 * @param one_station value of the element
 */
void list_append(List_t *one_list, char *one_key, Station_t *one_station);

/**
 * @brief Print an element
 *
 * @param one_element element to print
 */
void element_print(Element_t *one_element);

/**
 * @brief Print an station
 *
 * @param one_station station to print
 */
void station_print(Station_t *one_station);

/**
 * @brief Print a list
 *
 * @param one_list list to print
 */
void list_print(List_t *one_list);

/**
 * @brief Check if the list contains an element
 *
 * @param one_list list to check
 * @param one_key key of the element
 * @return bool true if the list contains the element, false otherwise
 */
bool list_contains(List_t *one_list, char *one_key);

/**
 * @brief Find an element in the list
 *
 * @param one_list list to search in
 * @param one_key key of the element
 * @return Station_t* value of the element if found, NULL otherwise
 */
Station_t *list_find(List_t *one_list, char *one_key);

/**
 * @brief Create a Station_t object
 * 
 * @param id id of the station
 * @param name name of the station
 * @param coordinates coordinates of the station
 * @param plugs_number number of plugs of the station
 * @param power power of the station
 * @param free free availability of the station
 * @return Station_t* 
 */
Station_t *station_create(int id, char *name, Coordinates_t *coordinates, int plugs_number, int power, bool free);

/** 
 * @brief Duplicate a station
 * 
 * @param one_station station to duplicate
 * @return Station_t* 
 */
Station_t *station_copy(Station_t *one_station);

/**
 * @brief Calculate the travel time between two stations in ticks
 *
 * @param one_station first station
 * @param other_station second station
 * @return int travel time in ticks
 */
int travel_ticks(Station_t *one_station, Station_t *other_station);

#endif /* __LIST_H__ */
