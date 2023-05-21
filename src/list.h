#include <stdbool.h>
#include "coordinates.h"

#ifndef __LIST_H__
#define __LIST_H__

#define VEHICLE_SPEED 60

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

List_t *list_create();

void list_destroy(List_t *one_list);

bool list_is_empty(List_t *one_list);

void list_append(List_t *one_list, char *one_key, Station_t *one_station);

void element_print(Element_t *one_element);

void station_print(Station_t *one_station);

void list_print(List_t *one_list);

bool list_contains(List_t *one_list, char *one_key);

Station_t *list_find(List_t *one_list, char *one_key);

Station_t *station_create(int id, char *name, Coordinates_t *coordinates, int plugs_number, int power, bool free);

Station_t *station_copy(Station_t *one_station);

double travel_time(Station_t *one_station, Station_t *other_station);

#endif /* __LIST_H__ */
