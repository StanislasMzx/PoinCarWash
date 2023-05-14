#ifndef SIMULATION_H
#define SIMULATION_H

#include "timeline2.h"

struct _Simulation_state_t
{
    Vehicle_t *vehicle;
    char *start;
    char *end;
    int start_time;
};
/**
 * @typedef Simulation_state_t
 * @brief Simulation state type
 *
 * @var Simulation_state::vehicle
 * One vehicle of the simulation
 * @var Simulation_state::start
 * Where the vehicle star
 * @var Simulation_state::end
 * Where the vehicle end
 * @var Simulation_state::start_time
 * Time when the vehicle start driving
 */
typedef struct _Simulation_state_t Simulation_state_t;


struct _Simulation_state_list_t
{
    Simulation_state_t *list;
    int length;
    int capacity;
};
/**
 * @typedef Simulation state
 * @brief Simulation state list type
 *
 * @var Simulation_state::list
 * List of simulation state
 * @var Simulation_state::length
 * Length of the list
 * @var Simulation_state::capacity
 * Capacity of the list
 */
typedef struct _Simulation_state_list_t Simulation_state_list_t;


Simulation_state_list_t *Simulation_state_list_empty();

bool Simulation_state_list_is_empty(Simulation_state_list_t* one_list);

void Simulation_state_list_append(Simulation_state_list_t* one_list, Simulation_state_t *one_state);

Timeline_t *simulation(Simulation_state_list_t *states);

#endif /* SIMULATION_H */
