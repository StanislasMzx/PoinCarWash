#define SNOW_ENABLED

#include "snow.h"
#include "../src/timeline_station.h"
#include <math.h>

describe(test_initializeTimelineUser)
{
    it("Conformance test")
    {
        Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");
        Timeline_all_users_t *global_timeline = initializeTimelineUser(table, "../data/input/network1.csv");
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
        timelineUserDestroyAll(&global_timeline);
        table_destroy(table);
    }
}

snow_main();
