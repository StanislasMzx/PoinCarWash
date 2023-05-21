#define SNOW_ENABLED

#include "snow.h"
#include "../src/timeline_station.h"
#include <math.h>

describe(test_initializeTimelineStation)
{
    // Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");

    // Timeline_all_stations_t *one_all_timeline = initializeTimelineAllStation(table);

    // asserteq_int(one_all_timeline->maxSize, table->nbStation);
    // for (int i=0; i<one_all_timeline->maxSize; i++){
    //     asserteq_int(one_all_timeline->listTimeline[i]->stateValue->tick, 0);
    //     assert(one_all_timeline->listTimeline[i]->next == NULL);
    // }

    // defer(table_destroy(table));
    // defer(destroyTimelineAllStations(one_all_timeline));
}

snow_main();
