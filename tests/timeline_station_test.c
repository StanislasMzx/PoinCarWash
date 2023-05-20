#define SNOW_ENABLED

#include "snow.h"
#include "../src/timeline.h"
#include <math.h>

describe(test_initializeTimelineStation)
{
    Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");

    free(table);
}

snow_main();
