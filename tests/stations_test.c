#define SNOW_ENABLED

#include "snow.h"
#include "../src/stations.h"

describe(test_load_stations)
{
    it("Conformance test")
    {
        Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230413.csv");

        assert(table_contains(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720"));
        asserteq_dbl(table_get(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720")->coordinates.longitude, 3.407609123225763);
        asserteq_dbl(table_get(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720")->coordinates.latitude, 43.41959147913006);
        asserteq_int(table_get(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720")->plugs_number, 1);
        asserteq_int(table_get(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720")->power, 22);
        assert(table_get(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720")->free);

        assert(table_contains(table, "FR3R3E10000849181"));
        asserteq_dbl(table_get(table, "FR3R3E10000849181")->coordinates.longitude, 4.055979);
        asserteq_dbl(table_get(table, "FR3R3E10000849181")->coordinates.latitude, 49.213929);
        asserteq_int(table_get(table, "FR3R3E10000849181")->plugs_number, 5);
        asserteq_int(table_get(table, "FR3R3E10000849181")->power, 150);
        assert(!table_get(table, "FR3R3E10000849181")->free);

        assert(table_contains(table, "Non concernébc3aedcc-3409-4038-89ed-4c3adad08708"));
        asserteq_dbl(table_get(table, "Non concernébc3aedcc-3409-4038-89ed-4c3adad08708")->coordinates.longitude, 2.3196844);
        asserteq_dbl(table_get(table, "Non concernébc3aedcc-3409-4038-89ed-4c3adad08708")->coordinates.latitude, 51.0175233);
        asserteq_int(table_get(table, "Non concernébc3aedcc-3409-4038-89ed-4c3adad08708")->plugs_number, 1);
        asserteq_int(table_get(table, "Non concernébc3aedcc-3409-4038-89ed-4c3adad08708")->power, 22);
        assert(!table_get(table, "Non concernébc3aedcc-3409-4038-89ed-4c3adad08708")->free);

        defer(table_destroy_after_stations_load(table));
    }
}

snow_main();
