#include "timeline_user.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "compute_trip.h"


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
void timelineUserPrepend(Timeline_user_t **one_timeline, int tick, char *station, int idStation, Vehicle_t *one_vehicle, List_t *one_trip, int one_stationsNumber)
{
    Timeline_user_t *node = malloc(sizeof(Timeline_user_t));
    assert(node != NULL);
    node->state = malloc(sizeof(User_state_t));
    assert(node->state != NULL);
    node->state->tick = tick;
    node->state->station = station;
    node->state->idStation = idStation;
    node->vehicle = one_vehicle;
    node->trip = one_trip;
    node->stationsNumber = one_stationsNumber;
    node->next = *one_timeline;
    if (node->next == NULL){
        node->state->stepTrip = 0;
    }
    else if (idStation == node->next->state->idStation){
        node->state->stepTrip = node->next->state->stepTrip;
    }else{
        node->state->stepTrip = node->next->state->stepTrip + 1;
    }
    *one_timeline = node;
}

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
void timelineUserAppend(Timeline_user_t **one_timeline, int tick, char *station, int idStation, Vehicle_t *one_vehicle, List_t *one_trip, int one_stationsNumber)
{
    if (*one_timeline == NULL)
    {
        timelineUserPrepend(one_timeline, tick, station, idStation, one_vehicle, one_trip, one_stationsNumber);
        return;
    }
    Timeline_user_t *tmp = *one_timeline;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    timelineUserPrepend(&tmp->next, tick, station, idStation, one_vehicle, one_trip, one_stationsNumber);
}

/**
 * @brief Get the user state at a given tick
 *
 * @param one_timeline Timeline to get the state from
 * @param tick Tick of the state
 * @return User_state_t* The user state
 */
User_state_t *timelineUserGet(Timeline_user_t *one_timeline, int tick)
{
    while (one_timeline->next != NULL && one_timeline->state->tick < tick)
    {
        one_timeline = one_timeline->next;
    }
    return one_timeline->state->tick == tick ? one_timeline->state : NULL;
}

/**
 * @brief Destroy a user timeline
 *
 * @param one_timeline Timeline to destroy
 */
void timelineUserDestroy(Timeline_user_t **one_timeline)
{
    Timeline_user_t *tmp = *one_timeline;
    while (tmp->next != NULL)
    {
        Timeline_user_t *next = tmp->next;
        vehicle_destroy(tmp->vehicle);
        free(tmp->state);
        free(tmp);
        tmp = next;
    }
    list_destroy(tmp->trip);
    vehicle_destroy(tmp->vehicle);
    free(tmp->state);
    free(tmp);
    *one_timeline = NULL;
}

/**
 * @brief Initialize a user timeline
 *
 * @param userNumber Number of users
 * @return Timeline_user_t* The user timeline
 */
Timeline_all_users_t *initializeTimelineUser(Table_t *station_table, char *network_file)
{
    FILE *fp = fopen(network_file, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m Incorrect file path.\n");
        exit(EXIT_FAILURE);
    }

    Timeline_all_users_t *one_timeline = malloc(sizeof(Timeline_all_users_t));
    assert(one_timeline != NULL);

    one_timeline->lastTick = -1;
    one_timeline->userNumber = 0;
    one_timeline->userArrived = 0;
    one_timeline->listTimeline = malloc(sizeof(Timeline_user_t));//malloc(sizeof(Timeline_user_t) * 1);
    assert(one_timeline->listTimeline != NULL);

    const unsigned max_line = 256;
    char line[max_line];

    fgets(line, 1024, fp);
    while (fgets(line, 1024, fp))
    {
        char departure[max_line];
        char arrival[max_line];
        char vehicleName[max_line];
        int departureTick;
        if (sscanf(line, "%[^,],%[^,],%[^,],%d\n", departure, arrival, vehicleName, &departureTick) != 4)
        {
            fprintf(stderr, "\33[31m>> Error:\33[0m Incorrect line format : %s.\n", line);
            continue;
        }
        Nominatim_t *departure_nominatim = nominatim_fetch(departure);
        Nominatim_t *arrival_nominatim = nominatim_fetch(arrival);
        Vehicle_t *vehicle = vehicle_find_by_name(vehicleName);
        if (vehicle->name == NULL)
        {
            continue;
        }
        Trip_output_t trip = compute_trip(station_table, departure_nominatim, arrival_nominatim, vehicle, 0.0, 360.0);
        nominatim_destroy(departure_nominatim);
        nominatim_destroy(arrival_nominatim);
        one_timeline->userNumber++;
        one_timeline->listTimeline = realloc(one_timeline->listTimeline, sizeof(Timeline_user_t)*one_timeline->userNumber);//sizeof(Timeline_user_t) * one_timeline->userNumber);
        one_timeline->listTimeline[one_timeline->userNumber - 1] = NULL;
        timelineUserPrepend(&one_timeline->listTimeline[one_timeline->userNumber - 1], departureTick, "", -1, vehicle, trip.trip, trip.trip->length);  // TODO: length of the linked list NOT trip.trip->length
        one_timeline->lastTick = departureTick > one_timeline->lastTick ? departureTick : one_timeline->lastTick;
        printf("\33[32m[~] Success:\33[0m User %d added.\n", one_timeline->userNumber);
    }

    fclose(fp);
    return one_timeline;
}

void timelineUserDestroyAll(Timeline_all_users_t **one_timeline)
{
    for (int i = 0; i < (*one_timeline)->userNumber; i++)
    {
        timelineUserDestroy(&(*one_timeline)->listTimeline[i]);
    }
    free((*one_timeline)->listTimeline);
    free(*one_timeline);
    *one_timeline = NULL;
}

/**
 * @brief Locate the last station of a user at a given tick
 *
 * @param one_timeline The user timeline
 * @param one_tick The tick
 * @return char* The station id string
 */
/*char *userLocation(Timeline_user_t *one_timeline, int one_tick)
{
    User_state_t *one_state = one_timeline->state;
    assert(one_state != NULL);

    if (one_tick < one_state->tick)
    {
        return NULL; // TODO: check behavior
    }

    return one_state->station;
}*/
int userLocation(Timeline_user_t *one_timeline, int one_tick, Table_t *table){
    User_state_t *one_state = one_timeline->state;
    assert(one_state != NULL);

    if (one_timeline->next != NULL && one_timeline->next->state->idStation == one_state->idStation){
        // on est parti d'une station
        Station_t *old_station = table_get(table, one_state->station);
        Station_t *new_station = one_timeline->trip->list[one_state->stepTrip].value;
        int travelTicks = travel_ticks(old_station, new_station);
        // TODO: check behavior
        int tick_arrived = one_state->tick + travelTicks;
        assert(tick_arrived <= one_tick);
        if (tick_arrived == one_tick){
            return new_station->id;
        }
    }
    return -1;
}
