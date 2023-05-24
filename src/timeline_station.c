#include "timeline_station.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "compute_trip.h"

/**
 * @brief Update the user timeline with the next tick
 *
 * @param one_timeline The user timeline
 */
// void nextTickUser(Timeline_all_users_t *user_timeline, Timeline_all_stations_t *station_timeline, Table_t *one_table)
// {
//     one_timeline->lastTick++;

//     for (int userId = 0; userId < one_timeline->userNumber; userId++)
//     {
//         Timeline_user_t *one_user_timeline = one_timeline->listTimeline[userId];
//         assert(one_user_timeline != NULL);

//         User_state_t *one_state = one_user_timeline->state;
//         assert(one_state != NULL);

//         // Update is scheduled for a later tick (>) or user has arrived (<)
//         if (one_state->tick != one_timeline->lastTick)
//         {
//             continue;
//         }

//         // Update is scheduled for this tick
//         Station_t *currentStation = list_find(one_user_timeline->trip, one_state->station); // ###### table_get(table, one_state->station);
//         assert(currentStation != NULL);

//         // Empty queue at the station
//         Station_state_t *station_state = NULL; // TODO: fetch Station_state_t of currentStation
//         if (station_state->availablePlugs > 0)
//         {
//             // Charge the vehicle
//             station_state->availablePlugs -= 1;

//             double chargingTime = 0; // TODO: compute charging time => USE PREVIOUS TIMELINE
//             // double distancePrev = distance(prevStation->coordinates, currentStation->coordinates);
//             // double chargingTime = (int)ceil((double)(distancePrev) / (double)(one_vehicle->fast_charge) * 60.0);
//             int next_tick = one_timeline->lastTick + (int)ceil(chargingTime / 10.0);

//             // TODO: add User_state_t with those values
//             one_state->tick = next_tick;
//             one_state->station = one_state->station;

//             // Plan next step
//             Station_t *nextStation = table_get(one_table, currentStation->last_station); // ######
//             assert(nextStation != NULL);

//             int travelTicks = (int)ceil(travel_time(currentStation, nextStation) / 10.0);
//             next_tick += travelTicks;

//             // TODO: add User_state_t with those values
//             char *nextStationId = currentStation->last_station; // TODO: fetch id of nextStation
//             one_state->tick = next_tick;
//             one_state->station = nextStationId;
//         }
//         // Queue at the station
//         else
//         {
//             // TODO: add 1 to station queue if user just arrived at the station (check previous timeline)
//             // TODO: add User_state_t with those values
//             one_state->tick = one_state->tick + 1;
//             one_state->station = one_state->station;
//         }
//     }
// }

void nextTickUser(Timeline_all_users_t *user_timeline, Timeline_all_stations_t *station_timeline, Table_t *one_table)
{
    assert(station_timeline->lastTick == user_timeline->lastTick);
    user_timeline->lastTick++;
    for (int i = 0; i < user_timeline->userNumber; i++)
    {
        User_state_t *user_state = user_timeline->listTimeline[i]->state;
        if (user_timeline->lastTick <= user_state->tick)
        {
            continue;
        }
        Timeline_user_t *old_timeline_user = user_timeline->listTimeline[i]->next;
        assert((old_timeline_user == NULL || user_state->idStation != old_timeline_user->state->idStation)); // not waiting at a station
        int loc = userLocation(user_timeline->listTimeline[i], i, user_timeline->lastTick, one_table);
        if (loc != -1)
        {
            char *new_station = station_timeline->listTimeline[loc]->name;
            timelineUserPrepend(&user_timeline->listTimeline[i], user_timeline->lastTick, new_station, loc, user_timeline->listTimeline[i]->vehicle, user_timeline->listTimeline[i]->trip, user_timeline->listTimeline[i]->stationsNumber + 1);
            if (user_timeline->listTimeline[i]->trip->length == user_timeline->listTimeline[i]->state->stepTrip)
            {
                user_timeline->userArrived++;
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
                station->id = nbUsedStations;
                nbUsedStations += 1;
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
                    one_timeline->name = station->name;
                    one_timeline->next = NULL;
                    one_timeline->statesNumber = 1;
                    one_timeline->stateValue = malloc(sizeof(Station_state_t));
                    assert(one_timeline->stateValue != NULL);
                    one_timeline->stateValue->availablePlugs = station->plugs_number;
                    one_timeline->stateValue->numberVehicle = 0;
                    one_timeline->stateValue->tick = 0;
                    one_timeline->stateValue->waitingTime = 0;
                    one_all_stations_timeline->listTimeline[station->id] = one_timeline;
                }
            }
        }
    }

    return one_all_stations_timeline;
}

/**
 * @brief add a station timeline
 *
 * @param one_all_station_timeline The all-stations timeline
 * @param one_name The name of the station to add
 * @param one_table The table of the station
 */
/*
void listTimelineAppend(Timeline_all_stations_t *one_all_stations_timeline, char *one_name, Table_t *one_table)
{
 int index = 0;

 while (one_all_stations_timeline->listTimeline[index] != NULL)
 {
     index++;
 }

 if (index == one_all_stations_timeline->nbStations)
 {
     one_all_stations_timeline->listTimeline = realloc(one_all_stations_timeline->listTimeline, sizeof(Timeline_station_t) * (one_all_stations_timeline->nbStations * 2));
     assert(one_all_stations_timeline->listTimeline != NULL);
 }

 one_all_stations_timeline->listTimeline[index] = createTimelineStation(one_name, one_table);
}*/

/**
 * @brief Initialize a station timeline
 *
 * @param one_name The name of the station
 * @param one_table The table of the station
 * @return Timeline_station_t* The station timeline
 */
/*
Timeline_station_t *createTimelineStation(char *one_name, Table_t *one_table)
{
    Timeline_station_t *one_timeline = malloc(sizeof(Timeline_station_t));
    assert(one_timeline != NULL);

    one_timeline->name = one_name;
    one_timeline->statesNumber = 1;

    Station_state_t *one_state = malloc(sizeof(Station_state_t));
    assert(one_state != NULL);

    one_state->tick = 0;
    one_state->numberVehicle = 0;
    one_state->availablePlugs = one_table->slots[table_indexof(one_table, one_name)]->list->value->plugs_number;
    one_state->waitingTime = 0;

    one_timeline->stateValue = one_state;
    one_timeline->next = NULL;

    return one_timeline;
}*/

/**
 * @brief find the index of a station timeline in the all-stations timeline
 *
 * @param one_all_stations_timeline The all-stations timeline
 * @param name The name of the station to find
 */
/*int stationTimelineGetIndex(Timeline_all_stations_t *one_all_stations_timeline, char *name)
{
    for (int index = 0; index < allStationsTimelineGetSize(one_all_stations_timeline); index++)
    {
        if (strcmp(one_all_stations_timeline->listTimeline[index]->name, name) == 0)
        {
            return index;
        }
    }

    return -1;
}*/

/**
 * @brief Get the size of the all-stations timeline
 *
 * @param one_all_stations_timeline The all-stations timeline
 * @return int The size of the all-stations timeline
 */
/*int allStationsTimelineGetSize(Timeline_all_stations_t *one_all_stations_timeline)
{
    int index = 0;

    while ((one_all_stations_timeline->listTimeline[index] != NULL) && (index < one_all_stations_timeline->nbStations))
    {
        index++;
    }

    return index;
}*/

/**
 * @brief Add a state to a station timeline
 *
 * @param one_timeline The station timeline
 * @param one_state The state to add
 */
/*void stationTimelineAddState(Timeline_station_t *one_timeline, Station_state_t *one_state)
{
    Timeline_station_t *current = one_timeline;

    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = malloc(sizeof(Timeline_station_t));
    assert(current->next != NULL);

    current->next->name = current->name;
    current->next->statesNumber = current->statesNumber + 1;
    current->next->stateValue = one_state;
    current->next->next = NULL;
}*/

/**
 * @brief Creates the state of a station for the next tick
 *
 * @param one_timeline The station timeline
 */

// void nextTickStation(Timeline_all_stations_t *station_timeline)
// {
//     for (int index = 0; index < station_timeline->nbStations; index++)
//     {
//         if (station_timeline->listTimeline[index] != NULL)
//         {
//             stationTimelineNextTick(station_timeline->listTimeline[index]);
//         }
//     }
// }

// void nextTickStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline){
//     nextTickUser(user_timeline);
//     for (int i=0; i<station_timeline->stationNumber; i++){
//         Timeline_station_t *one_timeline = station_timeline->listTimeline[i];
//         Timeline_station_t* new = malloc(sizeof(Timeline_station_t));
//         new->name = one_timeline->name;
//         new->statesNumber = one_timeline->statesNumber + 1;
//         new->stateValue = malloc(sizeof(Station_state_t));
//         new->stateValue->availablePlugs = one_timeline->stateValue->availablePlugs;
//         new->stateValue->numberVehicle = one_timeline->stateValue->numberVehicle;
//         new->stateValue->tick = one_timeline->stateValue->tick + 1;
//         if (one_timeline->stateValue->waitingTime > 0){
//             new->stateValue->waitingTime = one_timeline->stateValue->waitingTime - 1;
//         }else{
//             new->stateValue->waitingTime = 0;
//         }
//         new->next = one_timeline;
//         station_timeline->listTimeline[i] = new; // on vient d'ajouter une case
//     for (int i=0; i<user_timeline->userNumber; i++){
//         Timeline_user_t *one_timeline = user_timeline->listTimeline[i];
//         if (one_timeline->state->station != -1){
//             Station_state_t *one_state = station_timeline->listTimeline[one_timeline->state->station]->stateValue;
//             if (one_timeline->next != NULL && one_timeline->state->station == one_timeline->next->state->station) // on veut savoir si la voiture part ou arrive dans une station
//             {
//                 one_state->availablePlugs ++;
//                 one_state->numberVehicle --;
//             }else{
//                 one_state->availablePlugs --;
//                 one_state->numberVehicle ++;
//                 double v = MIN(one_timeline->vehicle->fast_charge, station_timeline->listTimeline[one_timeline->state->station]->power);
//                 one_state->waitingTime += (double) distance()
//             }
//         }
//     }
//     }
// }

// void nextTickStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline){
//     nextTickUser(user_timeline);
//     for (int i=0; i<station_timeline->stationNumber; i++){
//         Timeline_station_t *one_timeline = station_timeline->listTimeline[i];
//         Timeline_station_t* new = malloc(sizeof(Timeline_station_t));
//         new->name = one_timeline->name;
//         new->statesNumber = one_timeline->statesNumber + 1;
//         new->stateValue = malloc(sizeof(Station_state_t));
//         new->stateValue->availablePlugs = one_timeline->stateValue->availablePlugs;
//         new->stateValue->numberVehicle = one_timeline->stateValue->numberVehicle;
//         new->stateValue->tick = one_timeline->stateValue->tick + 1;
//         if (one_timeline->stateValue->waitingTime > 0){
//             new->stateValue->waitingTime = one_timeline->stateValue->waitingTime - 1;
//         }else{
//             new->stateValue->waitingTime = 0;
//         }
//         new->next = one_timeline;
//         station_timeline->listTimeline[i] = new; // on vient d'ajouter une case
//     for (int i=0; i<user_timeline->userNumber; i++){
//         Timeline_user_t *one_timeline = user_timeline->listTimeline[i];
//         if (one_timeline->state->station != -1){
//             Station_state_t *one_state = station_timeline->listTimeline[one_timeline->state->station]->stateValue;
//             if (one_timeline->next != NULL && one_timeline->state->station == one_timeline->next->state->station) // on veut savoir si la voiture part ou arrive dans une station
//             {
//                 one_state->availablePlugs ++;
//                 one_state->numberVehicle --;
//             }else{
//                 one_state->availablePlugs --;
//                 one_state->numberVehicle ++;
//                 double v = MIN(one_timeline->vehicle->fast_charge, station_timeline->listTimeline[one_timeline->state->station]->power);
//                 one_state->waitingTime += (double) distance()
//             }
//         }
//     }
//     }
// }

// void stationTimelineNextTick(Timeline_station_t *one_timeline)
// {
//     Station_state_t *one_state = malloc(sizeof(Station_state_t));
//     assert(one_state != NULL);

//     one_state->tick = one_timeline->stateValue->tick + 1;

//     one_state->numberVehicle = one_timeline->stateValue->numberVehicle; // will be calculated during the calculation of the next tick for the users

//     one_state->availablePlugs = one_timeline->stateValue->availablePlugs; // will be calculated during the calculation of the next tick for the users

//     one_state->waitingTime = 0; // will be calculated during the calculation of the next tick for the users

//     stationTimelineAddState(one_timeline, one_state);
// }

void nextTickStation(Timeline_all_stations_t *station_timeline, Timeline_all_users_t *user_timeline, Table_t *table)
{
    nextTickUser(user_timeline, station_timeline, table);
    station_timeline->lastTick++;
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
        if (one_user_timeline->state->idStation != -1)
        {
            Station_state_t *station_state = station_timeline->listTimeline[one_user_timeline->state->idStation]->stateValue;
            Timeline_user_t *current_user = one_user_timeline;
            while (current_user != NULL && current_user->state->tick == station_state->tick)
            {
                current_user = current_user->next;
            }
            if (current_user->next != NULL && current_user->state->idStation == current_user->next->state->idStation) // on veut savoir si la voiture part ou arrive dans une station
            {
                station_state->availablePlugs++;
                station_state->numberVehicle--;
            }
            else
            {
                station_state->availablePlugs--;
                station_state->numberVehicle++;
                double v = MIN((int)current_user->vehicle->fast_charge, station_timeline->listTimeline[current_user->state->idStation]->power);
                Station_t *oldTickStation = table_get(table, station_timeline->listTimeline[current_user->next->state->idStation]->name);
                Station_t *currentTickStation = table_get(table, station_timeline->listTimeline[current_user->state->idStation]->name);
                station_state->waitingTime += (double)distance(oldTickStation->coordinates, currentTickStation->coordinates) / v;
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
