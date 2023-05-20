#define SNOW_ENABLED

#include "snow.h"
#include "../src/timeline_user.h"
#include <math.h>

describe(test_initializeTimelineUser)
{
    Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");
    Timeline_all_users_t *global_timeline = initializeTimelineUser(table, "../data/input/network1.csv");
    timelineUserDestroyAll(&global_timeline);
    free(table);
}

snow_main();
