#define SNOW_ENABLED

#include "snow.h"
#include "../src/timeline_station.h"
#include <math.h>

describe(test_initializeTimelineStation)
{
    Table_t *table = load_stations(STATION_TABLE_PATH);

    Timeline_all_users_t *one_all_user = initializeTimelineUser(table, NETWORK_PATH);
    Timeline_all_stations_t *one_all_station = initializeTimelineAllStation(one_all_user, table);

    for (int i=0; i<one_all_station->nbStations; i++){
        asserteq_int(one_all_station->listTimeline[i]->stateValue->tick, 0);
        assert(one_all_station->listTimeline[i]->next == NULL);
        asserteq_int(one_all_user->listTimeline[i]->state->idStation, i);
    }

    defer(table_destroy(table));
    defer(destroyTimelineAllStations(one_all_station));
}

snow_main();
