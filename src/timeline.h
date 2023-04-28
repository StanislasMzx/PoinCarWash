#ifndef TIMELINE_H
#define TIMELINE_H

#include "table.h"
#include "stations.h"
#include "vehicles.h"

struct _Station_state_t
{
    int tick;
    int plugsNumber;
    int availablePlugs;
    Vehicle_list_t vehicles; // TODO: create type
};
/**
 * @typedef Station_state_t
 * @brief Station state type
 *
 * @var Station_state::tick
 * Tick of the state update
 * @var Station_state::plugsNumber
 * Number of plugs of the station
 * @var Station_state::availablePlugs
 * Number of available plugs of the station
 * @var Station_state::vehicles
 * List of vehicles charging at the station
 */
typedef struct _Station_state_t Station_state_t;

struct _Timeline_station_t
{
    char *name;
    Station_t *station;
    int statesNumber;
    Station_state_t *states;
};
/**
 * @typedef Timeline_station_t
 * @brief Timeline station type
 *
 * @var Timeline_station::name
 * Name of the station
 * @var Timeline_station::station
 * Pointer to the station
 * @var Timeline_station::statesNumber
 * Number of states of the station
 * @var Timeline_station::states
 * List of states of the station
 */

struct _Update_cache_t
{
    int tick;
    Station_state_hash_table_t *stationStates; // TODO: create type
};
/**
 * @typedef Update_cache_t
 * @brief Update cache type
 *
 * @var Update_cache::tick
 * Tick of the update
 * @var Update_cache::stationStates
 * Hash table of station states
 */
typedef struct _Update_cache_t Update_cache_t;

struct _Timeline_t
{
    int lastTick;
    Timeline_station_hash_table_t *stationStates; // TODO: create type
    Update_cache_t *nextUpdateCache;
};
/**
 * @typedef Timeline_t
 * @brief Timeline type
 *
 * @var Timeline::lastTick
 * Last tick of the timeline
 * @var Timeline::stationStates
 * Hash table of station states
 * @var Timeline::nextUpdateCache
 * Cache of the next update
 */
typedef struct _Timeline_t Timeline_t;

#endif /* TIMELINE_H */
