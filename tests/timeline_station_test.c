#define SNOW_ENABLED

#include "snow.h"
#include "../src/timeline_station.h"
#include <math.h>

describe(test_initializeTimelineStation)
{
    Table_t *table = load_stations(STATION_TABLE_PATH);

    Timeline_all_users_t *one_all_user = initializeTimelineUser(table, NETWORK_PATH);
    Timeline_all_stations_t *one_all_station = initializeTimelineAllStation(one_all_user, table);

    asserteq_int(one_all_station->nbStations, 9);
    for (int i=0; i<one_all_station->nbStations; i++){
        asserteq_int(one_all_station->listTimeline[i]->stateValue->tick, 0);
        assert(one_all_station->listTimeline[i]->next == NULL);   
    }

    assert(table_get(table, "FRFR1PRATIESIM0E")->id != -1);
    assert(table_get(table, "FRM38P38185022")->id != -1);
    assert(table_get(table, "FREBNPZFXETGZFXETG")->id != -1);
    assert(table_get(table, "FRFR1PAJAG0W3H7HAJAG0W3H7H")->id != -1);
    assert(table_get(table, "FRFR1PWD96MMJNF3WD96MMJNF3")->id != -1);
    assert(table_get(table, "FRS89PL4SV657FSZL4SV657FSZ")->id != -1);


    defer(table_destroy(table));
    defer(destroyTimelineAllStations(one_all_station));
}

snow_main();
