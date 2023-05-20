#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "table.h"
#include "stations.h"
#include "vehicles.h"
#include "timeline_user.h"

/**
 * @brief Initialize a user timeline
 * 
 * @param userNumber Number of users
 * @return Timeline_user_t* The user timeline
 */
Timeline_all_users_t *initializeTimelineUser(int userNumber)
{
    Timeline_all_users_t *one_timeline = malloc(sizeof(Timeline_all_users_t));

    assert(one_timeline != NULL);

    one_timeline->lastTick = -1;
    one_timeline->userNumber = userNumber;
    one_timeline->listTimeline = malloc(sizeof(Timeline_user_t *) * userNumber);
    assert(one_timeline->listTimeline != NULL);

    for (int userId = 0; userId < userNumber; userId++)
    {
        one_timeline->listTimeline[userId] = NULL;
    }

    return one_timeline;
}

/**
 * @brief Locate the last station of a user at a given tick
 * 
 * @param one_timeline The user timeline
 * @param one_tick The tick
 * @return char* The station id
*/
char *userLocation(Timeline_user_t *one_timeline, int one_tick)
{
    User_state_t *one_state = one_timeline->state;
    assert(one_state != NULL);

    if (one_tick < one_state->tick)
    {
        return NULL; // TODO: check behavior
    }

    return one_state->station;
}

/**
 * @brief Update the user timeline with the next tick
 * 
 * @param one_timeline The user timeline
 */
void nextTickUser(Timeline_all_users_t *one_timeline)
{
    one_timeline->lastTick++;

    for (int userId = 0; userId < one_timeline->userNumber; userId++)
    {
        Timeline_user_t *one_user_timeline = one_timeline->listTimeline[userId];
        assert(one_user_timeline != NULL);

        User_state_t *one_state = one_user_timeline->state;
        assert(one_state != NULL);

        // Update is scheduled for a later tick (>) or user has arrived (<)
        if (one_state->tick != one_timeline->lastTick)
        {
            continue;
        }

        // Update is scheduled for this tick
        Station_t *currentStation = list_find(one_user_timeline->trip, one_state->station);
        assert(currentStation != NULL);

        // Empty queue at the station
        Station_state_t *station_state; // TODO: fetch Station_state_t of currentStation
        if (station_state->availablePlugs > 0)
        {
            // Charge the vehicle
            station_state->availablePlugs -= 1;

            double chargingTime; // TODO: compute charging time
            // double distancePrev = distance(prevStation->coordinates, currentStation->coordinates);
            // double chargingTime = (int)ceil((double)(distancePrev) / (double)(one_vehicle->fast_charge) * 60.0);
            int next_tick = one_timeline->lastTick + (int)ceil(chargingTime / 10.0);

            // TODO: add User_state_t with those values
            one_state->tick = next_tick;
            one_state->station = one_state->station;

            // Plan next step
            Station_t *nextStation = currentStation->last_station;
            assert(nextStation != NULL);

            double travelTicks = (int)ceil(travel_time(currentStation->coordinates, nextStation->coordinates) / 10.0);
            next_tick += (int)ceil(travelTicks);

            // TODO: add User_state_t with those values
            char *nextStationId; // TODO: fetch id of nextStation
            one_state->tick = next_tick;
            one_state->station = nextStationId;
        }
        // Queue at the station
        else
        {
            // TODO: add User_state_t with those values
            one_state->tick = one_state->tick + 1;
            one_state->station = one_state->station;
        }
    }
}

/**
 * @brief Generate the whole user timeline
 * 
 * @param one_timeline The user timeline
 */
void makeTimelineUser(Timeline_all_users_t *one_timeline)
{
    bool allUsersArrived = false;

    while (!allUsersArrived)
    {
        nextTickUser(one_timeline);

        allUsersArrived = true;
        for (int userId = 0; userId < one_timeline->userNumber; userId++)
        {
            Timeline_user_t *one_user_timeline = one_timeline->listTimeline[userId];
            assert(one_user_timeline != NULL);

            User_state_t *one_state = one_user_timeline->state;
            assert(one_state != NULL);

            if (one_state->tick > one_timeline->lastTick)
            {
                allUsersArrived = false;
                break;
            }
        }
    }
}
