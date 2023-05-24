#include "timeline_station.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "compute_trip.h"


void _user_print(Timeline_user_t *user){
    Timeline_user_t *current = user;
    printf("\n\n\nPrint user: \n");
    while (current != NULL){
        printf("(tick: %d, station: %d)\n", current->state->tick, current->state->idStation);
        current = current->next;
    }
}


void nextTickUser(Timeline_all_users_t *user_timeline, Timeline_all_stations_t *station_timeline, Table_t *one_table)
{
    assert(station_timeline->lastTick == user_timeline->lastTick);
    user_timeline->lastTick++;
    for (int i = 0; i < user_timeline->userNumber; i++)
    {
        User_state_t *user_state = user_timeline->listTimeline[i]->state;
        Timeline_user_t *one_user_timeline = user_timeline->listTimeline[i];

        //printf("\nstep: %d, len: %d\n", user_state->stepTrip, user_timeline->listTimeline[i]->trip->length);
        
        if (user_state->stepTrip != user_timeline->listTimeline[i]->trip->length-1){
            if (user_timeline->lastTick <= user_state->tick)
            {
                continue;
            }
            Timeline_user_t *old_timeline_user = user_timeline->listTimeline[i]->next;
            assert((old_timeline_user == NULL || user_state->idStation == old_timeline_user->state->idStation)); // not waiting at a station
            int loc = userLocation(user_timeline->listTimeline[i], i, user_timeline->lastTick, one_table);


            if (loc > -1)
            {
                Timeline_station_t *one_station_timeline = station_timeline->listTimeline[loc];
                char *new_station = station_timeline->listTimeline[loc]->name;
                char *old_station;
                Station_t *before;
                if (user_state->stepTrip == 0){
                    old_station = one_table->slots[0]->list[i].key;
                    before = one_table->slots[0]->list[i].value;
                }else{
                    old_station = station_timeline->listTimeline[user_state->idStation]->name;
                    before = table_get(one_table, old_station);
                }
                Station_t *after = table_get(one_table, new_station);

                int step = user_state->stepTrip + 1;
                timelineUserPrepend(&user_timeline->listTimeline[i], user_timeline->lastTick, new_station, loc, user_timeline->listTimeline[i]->vehicle, user_timeline->listTimeline[i]->trip, user_timeline->listTimeline[i]->stationsNumber + 1, step);
                int travelTick = travel_ticks(before, after);
                /*if (one_station_timeline->stateValue->availablePlugs > 0){
                    one_station_timeline->stateValue->waitingTime += travelTick;
                }

                one_station_timeline->stateValue->numberVehicle ++;
                one*/

                // une voiture arrive dans une station

                int v = one_user_timeline->vehicle->fast_charge; //MIN((int)one_user_timeline->vehicle->fast_charge, one_station_timeline->power);
                one_station_timeline->stateValue->numberVehicle ++;
                int wait = (int)ceil(travelTick * VEHICLE_SPEED / v);
                /*if (user_timeline->lastTick < 40){
                    printf("\n\ntravel tick: %d, speed charge: %d, vehicle speed: %d, speed charge vehicle: %u, speed charge station: %d\n\n\n\n\n\n\n", travelTick, v, VEHICLE_SPEED, one_user_timeline->vehicle->fast_charge, one_station_timeline->power);
                }*/
                if (one_station_timeline->stateValue->availablePlugs > 1)
                {
                    one_station_timeline->stateValue->availablePlugs --;
                }
                else
                {
                    one_station_timeline->stateValue->waitingTime += wait;
                    wait = one_station_timeline->stateValue->waitingTime;
                }


                //printf("\nOn tick %d; User number %d have to wait %d tick in %s who have %d plugs and %d available. \n", user_timeline->lastTick, i, wait, table_get(one_table, one_station_timeline->name)->name, table_get(one_table, one_station_timeline->name)->plugs_number, one_station_timeline->stateValue->availablePlugs);


                timelineUserPrepend(&user_timeline->listTimeline[i], user_timeline->lastTick+wait, new_station, loc, user_timeline->listTimeline[i]->vehicle, user_timeline->listTimeline[i]->trip, user_timeline->listTimeline[i]->stationsNumber + 1, step);
                //printf("\nlen: %d == step: %d\n", user_timeline->listTimeline[i]->trip->length, user_timeline->listTimeline[i]->state->stepTrip);

            }
            else if(loc == -1){
                //printf("\n\n\nHERE\n\n\n\n");
                char *new_station;
                int step;
                if (user_state->stepTrip == 0){
                    new_station = one_table->slots[0]->list[i].key;
                    step = 1;
                }
                else{
                    assert(user_timeline->listTimeline[i]->trip->length == user_timeline->listTimeline[i]->state->stepTrip+2);
                    step = user_timeline->listTimeline[i]->state->stepTrip+1;
                    new_station = one_table->slots[1]->list[i].key;
                    user_timeline->userArrived++;
                }
                timelineUserPrepend(&user_timeline->listTimeline[i], user_timeline->lastTick, new_station, loc, user_timeline->listTimeline[i]->vehicle, user_timeline->listTimeline[i]->trip, user_timeline->listTimeline[i]->stationsNumber + 1, step);


            }
        }
    }
}

/**
 * @brief Generate the whole user timeline
 *
 * @param user_timeline The user timeline
 */
void makeTimelineUser(Timeline_all_users_t *user_timeline, Timeline_all_stations_t *station_timeline, Table_t *table)
{
    bool allUsersArrived = false;

    while (!allUsersArrived)
    {
        nextTickUser(user_timeline, station_timeline, table);

        allUsersArrived = true;
        for (int userId = 0; userId < user_timeline->userNumber; userId++)
        {
            Timeline_user_t *one_user_timeline = user_timeline->listTimeline[userId];
            assert(one_user_timeline != NULL);

            User_state_t *one_state = one_user_timeline->state;
            assert(one_state != NULL);

            if (one_state->tick > user_timeline->lastTick)
            {
                allUsersArrived = false;
                break;
            }
        }
    }
}

/**
 * @brief Initialize the station ids and return the number of used stations
 *
 * @param all_users_timeline The user timeline
 * @param one_table The table of stations
 * @return int The number of used stations
 */
int initializeStationIds(Timeline_all_users_t *all_users_timeline, Table_t *one_table)
{
    int nbUsedStations = 0;

    // Fetch all stations
    for (int idTimeline = 0; idTimeline < all_users_timeline->userNumber; idTimeline++)
    {
        Timeline_user_t *one_user_timeline = all_users_timeline->listTimeline[idTimeline];
        assert(one_user_timeline != NULL);
        List_t *trip = one_user_timeline->trip;
        assert(trip != NULL);
        Station_t *station;

        // Fetch all stations except start and end
        for (int i = 1; i < trip->length - 1; i++)
        {
            station = table_get(one_table, trip->list[i].key);
            assert(station != NULL);

            if (station->id == -1)
            {
                // Set station id
                trip->list[i].value->id = nbUsedStations;
                station->id = nbUsedStations;
                nbUsedStations += 1;
            }else{
                trip->list[i].value->id = station->id;
            }
        }
    }

    return nbUsedStations;
}

/**
 * @brief Initialize the all stations timeline
 *
 * @param all_users_timeline The all users timeline
 * @param one_table The table of the station
 * @return Timeline_station_t* The station timeline
 */
Timeline_all_stations_t *initializeTimelineAllStation(Timeline_all_users_t *all_users_timeline, Table_t *one_table)
{
    Timeline_all_stations_t *one_all_stations_timeline = malloc(sizeof(Timeline_all_stations_t));
    assert(one_all_stations_timeline != NULL);

    one_all_stations_timeline->nbStations = 0;
    one_all_stations_timeline->lastTick = all_users_timeline->lastTick;
    int nbUsedStations = initializeStationIds(all_users_timeline, one_table);
    one_all_stations_timeline->nbStations = nbUsedStations;
    one_all_stations_timeline->listTimeline = calloc(nbUsedStations, sizeof(Timeline_station_t));
    assert(one_all_stations_timeline->listTimeline != NULL);

    // Fetch all stations
    for (int idTimeline = 0; idTimeline < all_users_timeline->userNumber; idTimeline++)
    {
        Timeline_user_t *one_user_timeline = all_users_timeline->listTimeline[idTimeline];
        assert(one_user_timeline != NULL);
        List_t *trip = one_user_timeline->trip;
        assert(trip != NULL);
        Station_t *station;
        Timeline_station_t *one_timeline;

        // Fetch all stations except start and end
        for (int i = 1; i < trip->length - 1; i++)
        {
            station = table_get(one_table, trip->list[i].key);
            assert(station != NULL);

            if (station->id != -1)
            {
                one_timeline = one_all_stations_timeline->listTimeline[station->id];

                // Create station timeline
                if (one_timeline == NULL)
                {
                    one_timeline = malloc(sizeof(Timeline_station_t));
                    assert(one_timeline != NULL);
                    one_timeline->name = trip->list[i].key;
                    one_timeline->next = NULL;
                    one_timeline->statesNumber = 1;
                    one_timeline->stateValue = malloc(sizeof(Station_state_t));
                    assert(one_timeline->stateValue != NULL);
                    one_timeline->stateValue->availablePlugs = station->plugs_number;
                    one_timeline->stateValue->numberVehicle = 0;
                    one_timeline->stateValue->tick = one_all_stations_timeline->lastTick;
                    one_timeline->stateValue->waitingTime = 0;
                    one_all_stations_timeline->listTimeline[station->id] = one_timeline;
                }
            }
        }
    }

    return one_all_stations_timeline;
}



void nextTickStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline, Table_t *table)
{
    nextTickUser(user_timeline, station_timeline, table);
    station_timeline->lastTick++;
    /*if (user_timeline->lastTick < 40){
        printf("\nLASTTICK: %d\n", station_timeline->lastTick);
    }*/
    for (int i = 0; i < station_timeline->nbStations; i++)
    {
        Timeline_station_t *one_timeline = station_timeline->listTimeline[i];
        Timeline_station_t *new = malloc(sizeof(Timeline_station_t));
        new->name = one_timeline->name;
        new->statesNumber = one_timeline->statesNumber + 1;
        new->stateValue = malloc(sizeof(Station_state_t));
        new->stateValue->availablePlugs = one_timeline->stateValue->availablePlugs;
        new->stateValue->numberVehicle = one_timeline->stateValue->numberVehicle;
        new->stateValue->tick = one_timeline->stateValue->tick + 1;
        if (one_timeline->stateValue->waitingTime > 0)
        {
            new->stateValue->waitingTime = one_timeline->stateValue->waitingTime - 1;
        }
        else
        {
            new->stateValue->waitingTime = 0;
        }
        new->next = one_timeline;
        station_timeline->listTimeline[i] = new; // on vient d'ajouter une case
    }
    for (int i = 0; i < user_timeline->userNumber; i++)
    {
        Timeline_user_t *one_user_timeline = user_timeline->listTimeline[i];
        if (one_user_timeline->state->stepTrip != 0 && one_user_timeline->state->stepTrip != one_user_timeline->trip->length-1){
            /*if (user_timeline->lastTick < 40){
        printf("\nstep: %d, id: %d, endedTrip: %d\n", one_user_timeline->state->stepTrip, one_user_timeline->state->idStation, user_timeline->userArrived);
    }*/
            
            //
            Timeline_station_t *one_station_timeline = station_timeline->listTimeline[one_user_timeline->state->idStation];
            if (one_user_timeline->state->tick == station_timeline->lastTick)
            {
                //_user_print(one_user_timeline);
                // une voiture part d'une station
                one_station_timeline->stateValue->availablePlugs ++;
                one_station_timeline->stateValue->numberVehicle --;
            }
        }
        
    }
}

/**
 * @brief For a given station, print the timeline
 *
 * @param one_timeline The all stations timeline to read
 * @param one_station The station to read the timeline
 */
void readTimelineStation(Timeline_all_stations_t *one_timeline, Station_t *one_station)
{
    Timeline_station_t *current = one_timeline->listTimeline[one_station->id];

    printf("Station %s :\n", one_station->name);

    while (current != NULL)
    {
        printf("Tick : %d; Vehicles : %d; Available plugs : %d; Waiting Time : %d\n", current->stateValue->tick, current->stateValue->numberVehicle, current->stateValue->availablePlugs, current->stateValue->waitingTime);
        current = current->next;
    }
}

/**
 * @brief For a given tick, print the state of all the stations
 *
 * @param one_timeline The all stations timeline to read
 * @param one_tick The tick to read
 */
void readTimelineStationByTick(Timeline_all_stations_t *one_timeline, int one_tick)
{
    for (int index = 0; index < one_timeline->nbStations; index++)
    {
        Timeline_station_t *current = one_timeline->listTimeline[index];

        while (current != NULL && current->stateValue->tick != one_tick)
        {
            current = current->next;
        }

        if (current != NULL)
        {
            printf("Station %s : Vehicle : %d; Available Plugs : %d; Waiting Time : %d\n", current->name, current->stateValue->numberVehicle, current->stateValue->availablePlugs, current->stateValue->waitingTime);
        }
    }
}

/**
 * @brief Destroy the all-stations timeline
 *
 * @param one_all_stations_timeline The all-stations timeline to destroy
 */
void destroyTimelineAllStations(Timeline_all_stations_t *one_all_stations_timeline)
{
    for (int index = 0; index < one_all_stations_timeline->nbStations; index++)
    {
        if (one_all_stations_timeline->listTimeline[index] != NULL)
        {
            destroyTimelineStation(one_all_stations_timeline->listTimeline[index]);
        }
    }

    free(one_all_stations_timeline->listTimeline);
    free(one_all_stations_timeline);
}

/**
 * @brief destroy a station timeline
 *
 * @param one_timeline The station timeline to destroy
 */
void destroyTimelineStation(Timeline_station_t *one_timeline)
{
    Timeline_station_t *current = one_timeline;

    while (current->next != NULL)
    {
        Timeline_station_t *tmp = current->next;
        free(current->stateValue);
        //free(current->name);
        free(current);
        current = tmp;
    }
    free(current->stateValue);
    free(current);
}

void makeTimelineStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline, Table_t *table)
{
    while (user_timeline->userArrived != user_timeline->userNumber)
    {
        nextTickStation(station_timeline, user_timeline, table);
    }
}
