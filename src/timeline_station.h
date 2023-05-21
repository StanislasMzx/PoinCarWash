#ifndef TIMELINE_STATION_H
#define TIMELINE_STATION_H


#include "a_star.h"
#include "timeline_user.h"


struct _Station_state_t
{
    int tick;
    int numberVehicle;
    int availablePlugs;
    int waitingTime;
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
    int power;
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
typedef struct _Timeline_station_t Timeline_station_t;


struct _Timeline_all_stations_t
{
    int lastTick;
    int maxSize;
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


void nextTickUser(Timeline_all_users_t *one_timeline, Table_t *one_table);


Timeline_all_stations_t *initializeTimelineAllStation(Table_t *one_table);

//int stationTimelineGetIndex(Timeline_all_stations_t *one_all_stations_timeline, char *name);

//void listTimelineAppend(Timeline_all_stations_t *one_all_stations_timeline, char *one_name, Table_t *one_table);

//int allStationsTimelineGetSize(Timeline_all_stations_t *one_all_stations_timeline);

//Timeline_station_t *createTimelineStation(char *one_name, Table_t *one_table);

void nextTickStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline, Table_t *table);

//void stationTimelineNextTick(Timeline_station_t *one_timeline);

void stationTimelineAddState(Timeline_station_t *one_timeline, Station_state_t *one_state);

void makeTimelineStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline);

void readTimelineStation(Timeline_all_stations_t *one_timeline);

void readTimelineStationByTick(Timeline_all_stations_t *one_timeline, int one_tick);

void destroyTimelineAllStations(Timeline_all_stations_t *one_all_stations_timeline);

void destroyTimelineStation(Timeline_station_t *one_timeline);



#endif /* TIMELINE_STATION_H */
