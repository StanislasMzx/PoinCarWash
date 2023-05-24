#define SNOW_ENABLED

#include "snow.h"
#include "../src/timeline_station.h"
#include <math.h>
/*
describe(test_initializeTimelineUser)
{
    it("Conformance test")
    {
        Table_t *table = load_stations(STATION_TABLE_PATH);
        Timeline_all_users_t *global_timeline = initializeTimelineUser(table, NETWORK_PATH);
        asserteq_int(global_timeline->lastTick, 2);
        asserteq_int(global_timeline->userNumber, 3);
        asserteq_int(global_timeline->listTimeline[0]->state->tick, 2);
        asserteq_str(global_timeline->listTimeline[0]->trip->list->value->name, "Cannes, Grasse, Alpes-Maritimes, Provence-Alpes-Côte d'Azur, France métropolitaine, France");
        asserteq_str(global_timeline->listTimeline[0]->vehicle->name, "Peugeot e-208");
        asserteq_int(global_timeline->listTimeline[1]->state->tick, 0);
        asserteq_str(global_timeline->listTimeline[1]->trip->list->value->name, "Nancy, Meurthe-et-Moselle, Grand Est, France métropolitaine, France");
        asserteq_str(global_timeline->listTimeline[1]->vehicle->name, "Tesla Model 3");
        asserteq_int(global_timeline->listTimeline[2]->state->tick, 1);
        asserteq_str(global_timeline->listTimeline[2]->trip->list->value->name, "Paris, Île-de-France, France métropolitaine, France");
        asserteq_str(global_timeline->listTimeline[2]->vehicle->name, "Renault Zoe ZE50 R110");
        
        
        defer(timelineUserDestroyAll(&global_timeline));
        defer(table_destroy(table));
    }
}
*/

describe(test_user_localization){
    it("Conformance test"){
        Table_t *table = load_stations(STATION_TABLE_PATH);
        Timeline_all_users_t *global_timeline = initializeTimelineUser(table, NETWORK_PATH);
        Timeline_all_stations_t *station_timeline = initializeTimelineAllStation(global_timeline, table);

        printf("\n%d\n", userLocation(global_timeline->listTimeline[0], 0, 3, table));

        
        defer(destroyTimelineAllStations(station_timeline));
        defer(timelineUserDestroyAll(&global_timeline));
        defer(table_destroy(table));
    }
}


describe(test_user_prepend){
    it("Conformance test"){
        Table_t *table = load_stations(STATION_TABLE_PATH);
        Timeline_all_users_t *global_timeline = initializeTimelineUser(table, NETWORK_PATH);
        
        char *id = malloc(5);
        strcpy(id, "test");

        char *id2 = malloc(6);
        strcpy(id2, "test2");

        Vehicle_t *one_vehicle = vehicle_find_by_name("tesla");

        List_t *trip = global_timeline->listTimeline[0]->trip;
        List_t *trip2 = global_timeline->listTimeline[2]->trip;

        timelineUserPrepend(&global_timeline->listTimeline[0], 10, id, 5, one_vehicle, trip, 1);
        timelineUserPrepend(&global_timeline->listTimeline[1], 11, id2, 6, one_vehicle, trip2, 1);


        asserteq_int(global_timeline->lastTick, 2);
        asserteq_int(global_timeline->userNumber, 3);
        asserteq_int(global_timeline->listTimeline[0]->state->tick, 2);
        asserteq_str(global_timeline->listTimeline[0]->trip->list->value->name, "Cannes, Grasse, Alpes-Maritimes, Provence-Alpes-Côte d'Azur, France métropolitaine, France");
        asserteq_str(global_timeline->listTimeline[0]->vehicle->name, "Peugeot e-208");
        asserteq_int(global_timeline->listTimeline[1]->state->tick, 0);
        asserteq_str(global_timeline->listTimeline[1]->trip->list->value->name, "Nancy, Meurthe-et-Moselle, Grand Est, France métropolitaine, France");
        asserteq_str(global_timeline->listTimeline[1]->vehicle->name, "Tesla Model 3");
        asserteq_int(global_timeline->listTimeline[2]->state->tick, 1);
        asserteq_str(global_timeline->listTimeline[2]->trip->list->value->name, "Paris, Île-de-France, France métropolitaine, France");
        asserteq_str(global_timeline->listTimeline[2]->vehicle->name, "Renault Zoe ZE50 R110");

        defer(timelineUserDestroyAll(&global_timeline));
        defer(table_destroy(table));
    }
}


snow_main();
