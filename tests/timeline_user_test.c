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
        timelineUserDestroyAll(&global_timeline);
        table_destroy(table);
    }
}

snow_main();
