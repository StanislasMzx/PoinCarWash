#ifndef TIMELINE_USER_H
#define TIMELINE_USER_H


#include "a_star.h"


struct _User_state_t
{
    int tick;
    int idStation;
    char *station;
    int stepTrip;
};
/**
 * @typedef User_state_t
 * @brief User state type
 *
 * @var User_state::tick
 * Tick of the state update
 * @var User_state::idStation
 * Numeric id of station
 * @var User_state::station
 * Station where the user is at this tick
 * @var User_state::stepTrip
 * Step in the trip
 */
typedef struct _User_state_t User_state_t;

struct _Timeline_user_t
{
    User_state_t *state;
    Vehicle_t *vehicle;
    List_t *trip;
    int stationsNumber;
    struct _Timeline_user_t *next;
};
/**
 * @typedef Timeline_user_t
 * @brief Timeline User type
 *
 * @var Timeline_user::vehicle
 * Type of vehicle that the user use
 * @var Timeline_user::trip
 * List of station which is the trip
 * @var Timeline_user::stationsNumber
 * Number of stations in the trip
 * @var Timeline_::next
 * Next element of the linked list
 */
typedef struct _Timeline_user_t Timeline_user_t;

struct _Timeline_all_users_t
{
    int lastTick;
    int userNumber;
    int userArrived;
    Timeline_user_t **listTimeline;
};
/**
 * @typedef Timeline_t
 * @brief Timeline type
 *
 * @var Timeline::lastTick
 * Last tick of the timeline 
 * @var Timeline::userNumber
 * Number of user
 * @var Timeline::userArrived
 * Number of user who ended their trip
 * @var Timeline::listTimeline
 * Table of timeline of users
 */
typedef struct _Timeline_all_users_t Timeline_all_users_t;

/**
 * @brief Prepend a user state to a timeline
 *
 * @param one_timeline Timeline to prepend to
 * @param tick Tick of the state
 * @param station Station of the state
 * @param idStation Id of the station
 * @param one_vehicle Vehicle of the state
 * @param one_trip Trip of the state
 * @param one_stationsNumber Number of stations of the state
 */
void timelineUserPrepend(Timeline_user_t **one_timeline, int tick, char *station, int idStation, Vehicle_t *one_vehicle, List_t *one_trip, int one_stationsNumber, int stepTrip);

/**
 * @brief Append a user state to a timeline
 *
 * @param one_timeline Timeline to append to
 * @param tick Tick of the state
 * @param station Station of the state
 * @param idStation Id of the station
 * @param one_vehicle Vehicle of the state
 * @param one_trip Trip of the state
 * @param one_stationsNumber Number of stations of the state
 */
void timelineUserAppend(Timeline_user_t **one_timeline, int tick, char *station, int idStation, Vehicle_t *one_vehicle, List_t *one_trip, int one_stationsNumber, int stepTrip);

/**
 * @brief Get the user state at a given tick
 *
 * @param one_timeline Timeline to get the state from
 * @param tick Tick of the state
 * @return User_state_t* The user state
 */
User_state_t *timelineUserGet(Timeline_user_t *one_timeline, int tick);

/**
 * @brief Initialize a user timeline
 *
 * @param userNumber Number of users
 * @return Timeline_user_t* The user timeline
 */
Timeline_all_users_t *initializeTimelineUser(Table_t *station_table, char *network_file);

/**
 * @brief Destroy a user timeline
 *
 * @param one_timeline Timeline to destroy
 */
void timelineUserDestroy(Timeline_user_t **one_timeline);

/**
 * @brief Destroy all user timelines
 *
 * @param one_timeline Timeline to destroy
 */
void timelineUserDestroyAll(Timeline_all_users_t **one_timeline);

/**
 * @brief Locate the last station of a user at a given tick
 *
 * @param one_timeline The user timeline
 * @param one_tick The tick
 * @return char* The station id string
 */
int userLocation(Timeline_user_t *one_timeline, int nbCallToAStar, int one_tick, Table_t *table);


#endif /* TIMELINE_USER_H */
