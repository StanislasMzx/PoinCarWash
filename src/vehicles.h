#ifndef VEHICLES_H
#define VEHICLES_H

struct _Vehicle_t
{
    char *name;
    unsigned int range;
    unsigned int fast_charge;
};
/**
 * @typedef Vehicle_t
 * @brief Vehicle type
 *
 * @var Vehicle::name
 * Name of the vehicle
 * @var Vehicle::range
 * Range of the vehicle in km
 * @var Vehicle::fast_charge
 * Fast charge speed of the vehicle in km/h
 */
typedef struct _Vehicle_t Vehicle_t;


struct _Vehicle_list_t
{
    Vehicle_t *list;
    int length;
    int capacity;
};
/**
 * @typedef Vehicle list
 * @brief Vehicle list type
 *
 * @var Vehicle_list_t::list
 * List of vehicles
 * @var Vehicle_list_t::length
 * Length of the list
 * @var Vehicle_list_t::capacity
 * Capacity of the list
 */
typedef struct _Vehicle_list_t Vehicle_list_t;



Vehicle_t vehicle_find_by_name(char *name);

Vehicle_t vehicle_input();

#endif /* VEHICLES_H */
