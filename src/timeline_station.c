#include "timeline_station.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "compute_trip.h"



int _find_plug(int *waitingTime, int number_plugs)
{
    int min = 0;
    for (int i=0; i<number_plugs; i++)
    {
        if (waitingTime[min] > waitingTime[i])
        {
            min = i;
        }
    }
    return min;
}


/**
 * @brief Update the user timeline with the next tick
 *
 * @param one_timeline The user timeline
 */
void _user_print(Timeline_user_t *user)
{
    Timeline_user_t *current = user;
    printf("\n\n\nPrint user: \n");
    while (current != NULL)
    {
        printf("(tick: %d, station: %d)\n", current->state->tick, current->state->idStation);
        current = current->next;
    }
}

void nextTickUser(Timeline_all_users_t *user_timeline, Timeline_all_stations_t *station_timeline, Table_t *one_table)
{
    user_timeline->lastTick++;
    assert(station_timeline->lastTick == user_timeline->lastTick);
    
    for (int i = 0; i < user_timeline->userNumber; i++)
    {


        User_state_t *user_state = user_timeline->listTimeline[i]->state;
        Timeline_user_t *one_user_timeline = user_timeline->listTimeline[i];



        if (user_state->stepTrip != user_timeline->listTimeline[i]->trip->length - 1)
        {
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
                if (user_state->stepTrip == 0)
                {
                    old_station = one_table->slots[0]->list[i].key;
                    before = one_table->slots[0]->list[i].value;
                }
                else
                {
                    old_station = station_timeline->listTimeline[user_state->idStation]->name;
                    before = table_get(one_table, old_station);
                }
                Station_t *after = table_get(one_table, new_station);

                int step = user_state->stepTrip + 1;
                timelineUserPrepend(&user_timeline->listTimeline[i], user_timeline->lastTick, new_station, loc, user_timeline->listTimeline[i]->vehicle, user_timeline->listTimeline[i]->trip, user_timeline->listTimeline[i]->stationsNumber + 1, step);
                int travelTick = travel_ticks(before, after);

                // une voiture arrive dans une station

                int plugs = one_station_timeline->stateValue->number_plugs;
                int *waiting = one_station_timeline->stateValue->waitingTime;
                int index = _find_plug(waiting, plugs);


                double v = one_user_timeline->vehicle->fast_charge;
                one_station_timeline->stateValue->numberVehicle++;
                int wait = (int)ceil(travelTick * VEHICLE_SPEED / v);
                
                waiting[index] += wait;


                timelineUserPrepend(&user_timeline->listTimeline[i], user_timeline->lastTick + waiting[index], new_station, loc, user_timeline->listTimeline[i]->vehicle, user_timeline->listTimeline[i]->trip, user_timeline->listTimeline[i]->stationsNumber + 1, step);

            }
            else if (loc == -1)
            {
                char *new_station;
                int step;
                if (user_state->stepTrip == 0)
                {
                    new_station = one_table->slots[0]->list[i].key;
                    step = 1;
                }
                else
                {
                    assert(user_timeline->listTimeline[i]->trip->length == user_timeline->listTimeline[i]->state->stepTrip + 2);
                    step = user_timeline->listTimeline[i]->state->stepTrip + 1;
                    new_station = one_table->slots[1]->list[i].key;
                    user_timeline->userArrived++;
                }
                timelineUserPrepend(&user_timeline->listTimeline[i], user_timeline->lastTick, new_station, loc, user_timeline->listTimeline[i]->vehicle, user_timeline->listTimeline[i]->trip, user_timeline->listTimeline[i]->stationsNumber + 1, step);
            }
        }
    }
}

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
            }
        }
    }

    return nbUsedStations;
}

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
                    one_timeline->stateValue->numberVehicle = 0;
                    one_timeline->stateValue->tick = one_all_stations_timeline->lastTick;
                    one_timeline->stateValue->number_plugs = station->plugs_number;
                    one_timeline->stateValue->waitingTime = calloc(station->plugs_number, sizeof(int));
                    for (int i=0; i<station->plugs_number; i++)
                    {
                        one_timeline->stateValue->waitingTime[i] = 0;
                    }
                    one_all_stations_timeline->listTimeline[station->id] = one_timeline;
                }
            }
        }
    }

    return one_all_stations_timeline;
}

void nextTickStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline, Table_t *table)
{
    station_timeline->lastTick++;

    for (int i = 0; i < station_timeline->nbStations; i++)
    {
        Timeline_station_t *one_timeline = station_timeline->listTimeline[i];
        Timeline_station_t *new = malloc(sizeof(Timeline_station_t));
        new->name = one_timeline->name;
        new->statesNumber = one_timeline->statesNumber + 1;
        new->stateValue = malloc(sizeof(Station_state_t));
        new->stateValue->numberVehicle = one_timeline->stateValue->numberVehicle;
        new->stateValue->tick = one_timeline->stateValue->tick + 1;
        new->stateValue->number_plugs = one_timeline->stateValue->number_plugs;
        new->stateValue->waitingTime = calloc(one_timeline->stateValue->number_plugs, sizeof(int));


        for (int i=0; i<new->stateValue->number_plugs; i++)
        {
            if (one_timeline->stateValue->waitingTime[i] > 0)
            {
                new->stateValue->waitingTime[i] = one_timeline->stateValue->waitingTime[i] - 1;
            }
            else
            {
                new->stateValue->waitingTime[i] = 0;
            }
        }
        
        
        new->next = one_timeline;
        station_timeline->listTimeline[i] = new; // on vient d'ajouter une case
    }

    
    nextTickUser(user_timeline, station_timeline, table);


    for (int i = 0; i < user_timeline->userNumber; i++)
    {
        Timeline_user_t *one_user_timeline = user_timeline->listTimeline[i];
        if (one_user_timeline->state->stepTrip != 0 && one_user_timeline->state->stepTrip != one_user_timeline->trip->length - 1)
        {

            //
            Timeline_station_t *one_station_timeline = station_timeline->listTimeline[one_user_timeline->state->idStation];
            if (one_user_timeline->state->tick == station_timeline->lastTick)
            {
                //_user_print(one_user_timeline);
                // une voiture part d'une station

                one_station_timeline->stateValue->numberVehicle--;
            }
        }
    }
}

void makeTimelineStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline, Table_t *table)
{
    while (user_timeline->userArrived != user_timeline->userNumber)
    {
        // printf("%d/%d\n", user_timeline->userArrived, user_timeline->userNumber);
        nextTickStation(station_timeline, user_timeline, table);
    }
}

void readTimelineStation(Timeline_all_stations_t *one_timeline, Station_t *one_station)
{
    Timeline_station_t *current = one_timeline->listTimeline[one_station->id];

    printf("\nStation %s :\n", one_station->name);

    while (current != NULL)
    {
        printf("Tick : %d; Vehicles : %d; number of plugs : %d; Waiting Time :\n", current->stateValue->tick, current->stateValue->numberVehicle, current->stateValue->number_plugs);
        for (int i=0; i<current->stateValue->number_plugs; i++)
        {
            printf("plug %d: %d\n", i, current->stateValue->waitingTime[i]);
        }
        current = current->next;
    }
}

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
            printf("Station %s : Vehicle : %d; Available Plugs : %d; Waiting Time : %d\n", current->name, current->stateValue->numberVehicle, current->stateValue->number_plugs, current->stateValue->waitingTime[0]);
        }
    }
}

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

void destroyTimelineStation(Timeline_station_t *one_timeline)
{
    Timeline_station_t *current = one_timeline;

    while (current->next != NULL)
    {
        Timeline_station_t *tmp = current->next;
        free(current->stateValue->waitingTime);
        free(current->stateValue);
        // free(current->name);
        free(current);
        current = tmp;
    }
    free(current->stateValue->waitingTime);
    free(current->stateValue);
    free(current);
}
