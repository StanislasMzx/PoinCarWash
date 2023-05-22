#ifndef TIMELINE_USER_H
#define TIMELINE_USER_H


#include "a_star.h"


struct _User_state_t
{
    int tick;
    int idStation;
    char *station;
};
/**
 * @typedef User_state_t
 * @brief User state type
 *
 * @var User_state::tick
 * Tick of the state update
 * @var User_state::station
 * Station where the user is at this tick
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
 * @var Timeline::listTimeline
 * Table of timeline of users
 */
typedef struct _Timeline_all_users_t Timeline_all_users_t;



void timelineUserPrepend(Timeline_user_t **one_timeline, int tick, char *station, int idStation, Vehicle_t *one_vehicle, List_t *one_trip, int one_stationsNumber);

void timelineUserAppend(Timeline_user_t **one_timeline, int tick, char *station, int idStation, Vehicle_t *one_vehicle, List_t *one_trip, int one_stationsNumber);

User_state_t *timelineUserGet(Timeline_user_t *one_timeline, int tick);

void timelineUserDestroy(Timeline_user_t **one_timeline);

Timeline_all_users_t *initializeTimelineUser(Table_t *station_table, char *network_file);

void timelineUserDestroyAll(Timeline_all_users_t **one_timeline);

char *userLocation(Timeline_user_t *one_timeline, int one_tick);

void makeTimelineUser(Timeline_all_users_t *one_timeline, Table_t *table);


#endif /* TIMELINE_USER_H */
