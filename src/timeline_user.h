#ifndef TIMELINE_USER_H
#define TIMELINE_USER_H

#ifndef INCLUDE_STATION_TIMELINE
#define INCLUDE_STATION_TIMELINE
#include "timeline_station.h"
#endif // INCLUDE_STATION_TIMELINE


#include "table.h"
#include "stations.h"
#include "vehicles.h"

struct _User_state_t
{
    int tick;
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
    // User_state_t linked list?
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

Timeline_all_users_t *initializeTimelineUser(int userNumber);

char *userLocation(Timeline_user_t *one_timeline, int one_tick);

void nextTickUser(Timeline_all_users_t *one_timeline);

void makeTimelineUser(Timeline_all_users_t *one_timeline);

#endif /* TIMELINE_USER_H */
