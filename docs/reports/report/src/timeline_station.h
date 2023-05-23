#ifndef TIMELINE_STATION_H
#define TIMELINE_STATION_H

#include "table.h"
#include "stations.h"
#include "vehicles.h"

struct _Station_state_t
{
    int tick;
    int numberVehicle;
    int availablePlugs;
};
/**
 * @typedef Station_state_t
 * @brief Station state type
 *
 * @var Station_state::tick
 * Tick of the state update
 * @var Station_state::numberVehicle
 * Number of vehicle at the station
 * @var Station_state::availablePlugs
 * Number of available plugs of the station
 */
typedef struct _Station_state_t Station_state_t;

struct _Timeline_station_t
{
    char *name;
    int statesNumber;
    Station_state_t *stateValue;
    struct _Timeline_station_t *next;
};
/**
 * @typedef Timeline_station_t
 * @brief Timeline station type
 *
 * @var Timeline_station::name
 * Name of the station
 * @var Timeline_station::statesNumber
 * Number of states of the station
 * @var Timeline_station::next
 * Next element of the linked list
 */
typedef _Timeline_station_t Timeline_station_t;


struct _Timeline_all_stations_t
{
    int lastTick;
    Timeline_station_t **listTimeline;
};
/**
 * @typedef Timeline_t
 * @brief Timeline type
 *
 * @var Timeline::lastTick
 * Last tick of the timeline
 * @var Timeline::listTimeline
 * Table of timeline of stations
 */
typedef struct _Timeline_all_stations_t Timeline_all_stations_t;

Timeline_all_stations_t *initializeTimelineStation();

void nextTickStation(Timeline_all_stations_t *one_timeline);

void makeTimelineStation(Timeline_all_stations_t *one_timeline);

void readTimelineStation(Timeline_all_stations_t *one_timeline);

void readTimelineStationByTick(Timeline_all_stations_t *one_timeline, int one_tick);


#endif /* TIMELINE_STATION_H */
