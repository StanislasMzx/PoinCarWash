#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "compute_trip.h"
#include "timeline.h"



// ##########################################################TIMELINE STATION #########################################################

Timeline_all_stations_t *initializeTimelineStation(Timeline_all_users_t *user_timeline, Table_t *one_table){
    Timeline_all_stations_t *station_timeline = malloc(sizeof(Timeline_all_stations_t));
    station_timeline->listTimeline = calloc(one_table->nbStation, sizeof(Timeline_station_t));
    station_timeline->stationNumber = one_table->nbStation;
    Station_t *station;
    Timeline_station_t *one_timeline;
    for (int i=0; i<one_table->length; i++){
        List_t *one_list = one_table->slots[i];
        for (int j=0; j<one_list->length; j++){
            station = one_list->list[j].value;
            one_timeline = station_timeline->listTimeline[station->id];
            one_timeline->name = one_list->list[j].key;
            one_timeline->next = NULL;
            one_timeline->statesNumber = 0;
            one_timeline->stateValue = malloc(sizeof(Station_state_t));
            one_timeline->stateValue->availablePlugs = station->plugs_number;
            one_timeline->stateValue->numberVehicle = 0;
            one_timeline->stateValue->tick = 0;
            one_timeline->stateValue->waitingTime = 0;
        }
    }
    return station_timeline;
}


void nextTickStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline){
    nextTickUser(user_timeline);
    for (int i=0; i<station_timeline->stationNumber; i++){
        Timeline_station_t *one_timeline = station_timeline->listTimeline[i];
        Timeline_station_t* new = malloc(sizeof(Timeline_station_t));
        new->name = one_timeline->name;
        new->statesNumber = one_timeline->statesNumber + 1;
        new->stateValue = malloc(sizeof(Station_state_t));
        new->stateValue->availablePlugs = one_timeline->stateValue->availablePlugs;
        new->stateValue->numberVehicle = one_timeline->stateValue->numberVehicle;
        new->stateValue->tick = one_timeline->stateValue->tick + 1;
        if (one_timeline->stateValue->waitingTime > 0){
            new->stateValue->waitingTime = one_timeline->stateValue->waitingTime - 1;
        }else{
            new->stateValue->waitingTime = 0;
        }
        new->next = one_timeline;
        station_timeline->listTimeline[i] = new; // on vient d'ajouter une case
    for (int i=0; i<user_timeline->userNumber; i++){
        Timeline_user_t *one_timeline = user_timeline->listTimeline[i];
        if (one_timeline->state->station != -1){
            Station_state_t *one_state = station_timeline->listTimeline[one_timeline->state->station]->stateValue;
            if (one_timeline->next != NULL && one_timeline->state->station == one_timeline->next->state->station) // on veut savoir si la voiture part ou arrive dans une station
            {
                one_state->availablePlugs ++;
                one_state->numberVehicle --;
            }else{
                one_state->availablePlugs --;
                one_state->numberVehicle ++;
                double v = MIN(one_timeline->vehicle->fast_charge, station_timeline->listTimeline[one_timeline->state->station]->power);
                one_state->waitingTime += (double) distance()
            }
        }
    }
    }
}




// ##########################################################TIMELINE USER #########################################################

/**
 * @brief Prepend a user state to a timeline
 *
 * @param one_timeline Timeline to prepend to
 * @param tick Tick of the state
 * @param station Station of the state
 * @param one_vehicle Vehicle of the state
 * @param one_trip Trip of the state
 * @param one_stationsNumber Number of stations of the state
 */
void timelineUserPrepend(Timeline_user_t **one_timeline, int tick, char *station, Vehicle_t *one_vehicle, List_t *one_trip, int one_stationsNumber)
{
    Timeline_user_t *node = malloc(sizeof(Timeline_user_t));
    assert(node != NULL);
    node->state = malloc(sizeof(User_state_t));
    assert(node->state != NULL);
    node->state->tick = tick;
    node->state->station = station;
    node->vehicle = one_vehicle;
    node->trip = one_trip;
    node->stationsNumber = one_stationsNumber;
    node->next = *one_timeline;
    *one_timeline = node;
}

/**
 * @brief Append a user state to a timeline
 *
 * @param one_timeline Timeline to append to
 * @param tick Tick of the state
 * @param station Station of the state
 * @param one_vehicle Vehicle of the state
 * @param one_trip Trip of the state
 * @param one_stationsNumber Number of stations of the state
 */
void timelineUserAppend(Timeline_user_t **one_timeline, int tick, char *station, Vehicle_t *one_vehicle, List_t *one_trip, int one_stationsNumber)
{
    if (*one_timeline == NULL)
    {
        list_prepend(one_timeline, tick, station, one_vehicle, one_trip, one_stationsNumber);
        return;
    }
    Timeline_user_t *tmp = *one_timeline;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    list_prepend(&tmp->next, tick, station, one_vehicle, one_trip, one_stationsNumber);
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
        free(tmp->vehicle);
        free(tmp->state);
        free(tmp);
        tmp = next;
    }
    free(tmp->vehicle);
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
    one_timeline->listTimeline = malloc(sizeof(Timeline_user_t *) * 1);
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
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^\n]", departure, arrival, vehicleName, departureTick) != 4)
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
        vehicle_destroy(vehicle);
        one_timeline->userNumber++;
        realloc(one_timeline->listTimeline, sizeof(Timeline_user_t *) * one_timeline->userNumber);
        timelineUserAppend(&one_timeline->listTimeline[one_timeline->userNumber - 1], departureTick, NULL, vehicle, trip.trip, trip.trip->length);
        one_timeline->lastTick = departureTick > one_timeline->lastTick ? departureTick : one_timeline->lastTick;
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

            double chargingTime; // TODO: compute charging time => USE PREVIOUS TIMELINE
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
            // TODO: add 1 to station queue if user just arrived at the station (check previous timeline)
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
