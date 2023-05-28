#ifndef TIMELINE_STATION_H
#define TIMELINE_STATION_H


#include "a_star.h"
#include "timeline_user.h"

#define NETWORK_PATH "../data/input/network1.csv"

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
    int nbStations;
    Timeline_station_t **listTimeline;
};
/**
 * @typedef Timeline_t
 * @brief Timeline type
 *
 * @var Timeline::lastTick
 * Last tick of the timeline
 * @var Timeline::nbStations
 * Number of stations in the timeline
 * @var Timeline::listTimeline
 * Table of timeline of stations
 */
typedef struct _Timeline_all_stations_t Timeline_all_stations_t;

/**
 * @brief Update the user timeline with the next tick
 *
 * @param one_timeline The user timeline
 */
void nextTickUser(Timeline_all_users_t *user_timeline, Timeline_all_stations_t *station_timeline, Table_t *one_table);

/**
 * @brief Generate the whole user timeline
 *
 * @param user_timeline The user timeline
 */
void makeTimelineUser(Timeline_all_users_t *user_timeline, Timeline_all_stations_t *station_timeline, Table_t *table);

/**
 * @brief Initialize the station ids and return the number of used stations
 *
 * @param all_users_timeline The user timeline
 * @param one_table The table of stations
 * @return int The number of used stations
 */
int initializeStationIds(Timeline_all_users_t *all_users_timeline, Table_t *one_table);

/**
 * @brief Initialize the all stations timeline
 *
 * @param all_users_timeline The all users timeline
 * @param one_table The table of the station
 * @return Timeline_station_t* The station timeline
 */
Timeline_all_stations_t *initializeTimelineAllStation(Timeline_all_users_t *all_users_timeline, Table_t *one_table);

/**
 * @brief Creates the state of a station for the next tick
 *
 * @param one_timeline The station timeline
 */
void nextTickStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline, Table_t *table);

/**
 * @brief Generate the whole station timeline
 *
 * @param station_timeline The station timeline
 */
void makeTimelineStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline, Table_t *table);

/**
 * @brief For a given station, print the timeline
 *
 * @param one_timeline The all stations timeline to read
 * @param one_station The station to read the timeline
 */
void readTimelineStation(Timeline_all_stations_t *one_timeline, Station_t *one_station);

/**
 * @brief For a given tick, print the state of all the stations
 *
 * @param one_timeline The all stations timeline to read
 * @param one_tick The tick to read
 */
void readTimelineStationByTick(Timeline_all_stations_t *one_timeline, int one_tick);

/**
 * @brief Destroy the all-stations timeline
 *
 * @param one_all_stations_timeline The all-stations timeline to destroy
 */
void destroyTimelineAllStations(Timeline_all_stations_t *one_all_stations_timeline);

/**
 * @brief destroy a station timeline
 *
 * @param one_timeline The station timeline to destroy
 */
void destroyTimelineStation(Timeline_station_t *one_timeline);



#endif /* TIMELINE_STATION_H */
