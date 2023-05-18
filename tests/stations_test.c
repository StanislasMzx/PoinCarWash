#define SNOW_ENABLED

#include "snow.h"
#include "../src/stations.h"

describe(test_load_stations)
{
    it("Conformance test")
    {
        Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");
        assert(table_contains(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720"));
        asserteq_str(table_get(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720")->name, "Hotel saint alban");
        asserteq_dbl(table_get(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720")->coordinates->longitude, 3.407609123225763);
        asserteq_dbl(table_get(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720")->coordinates->latitude, 43.41959147913006);
        asserteq_int(table_get(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720")->plugs_number, 1);
        asserteq_int(table_get(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720")->power, 22);
        assert(table_get(table, "FR00001106217402af2893-256f-4124-8d58-c728dc38f720")->free);

        assert(table_contains(table, "FR3R3P89258297"));
        asserteq_str(table_get(table, "FR3R3P89258297")->name, "R3 - Norauto Reims Cormontreuil");
        asserteq_dbl(table_get(table, "FR3R3P89258297")->coordinates->longitude, 4.055979);
        asserteq_dbl(table_get(table, "FR3R3P89258297")->coordinates->latitude, 49.213929);
        asserteq_int(table_get(table, "FR3R3P89258297")->plugs_number, 5);
        asserteq_int(table_get(table, "FR3R3P89258297")->power, 22);
        assert(!table_get(table, "FR3R3P89258297")->free);

        assert(table_contains(table, "Non concerné8cec9f22-53ca-40ba-9fc4-c58494ecc43c"));
        asserteq_str(table_get(table, "Non concerné8cec9f22-53ca-40ba-9fc4-c58494ecc43c")->name, "PFMV - GRANDE SYNTHE");
        asserteq_dbl(table_get(table, "Non concerné8cec9f22-53ca-40ba-9fc4-c58494ecc43c")->coordinates->longitude, 2.3196844);
        asserteq_dbl(table_get(table, "Non concerné8cec9f22-53ca-40ba-9fc4-c58494ecc43c")->coordinates->latitude, 51.0175233);
        asserteq_int(table_get(table, "Non concerné8cec9f22-53ca-40ba-9fc4-c58494ecc43c")->plugs_number, 1);
        asserteq_int(table_get(table, "Non concerné8cec9f22-53ca-40ba-9fc4-c58494ecc43c")->power, 22);
        assert(!table_get(table, "Non concerné8cec9f22-53ca-40ba-9fc4-c58494ecc43c")->free);

        defer(table_destroy(table));
    }
}


describe(test_reinitialize_table)
{
    it("Conformance test")
    {
        Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");

        int N = 100; // number of changes
        int index;
        for (int i=0; i<N; i++){
            index = rand()%N;
            List_t *one_list = table->slots[index];
            for (int j=0; j<one_list->length; j++){
                one_list->list[j].value->weight = (double) index;
                one_list->list[j].value->last_station = "";
            }
        }
        reinitializeTable(table);
        for (int i=0; i<table->length; i++){
            List_t *one_list = table->slots[i];
            for (int j=0; j<one_list->length; j++){
                asserteq_dbl(one_list->list[j].value->weight, -1);
                assert(one_list->list[j].value->last_station == NULL);
            }
        }

        defer(table_destroy(table));
    }
}
describe(test_reachable_station_neighbors)
{
    it("Conformance test")
    {
        Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");

        List_t *list = reachable_station_neighbors(table, "FRFR1PRYXKLFRYXKLF", 10);
        asserteq_int(list->length, 3);
        assert(list_contains(list, "FRFR1PTFPYCFK3HITFPYCFK3HI"));
        assert(list_contains(list, "FRFR1PZO7EL7PRD6ZO7EL7PRD6"));
        assert(list_contains(list, "FRFR1PLX3M7GUQLZLX3M7GUQLZ"));
        free(list->list);
        free(list);

        list = reachable_station_neighbors(table, "FRQOVP5400004FR*QOV*54-00004", 1);
        asserteq_int(list->length, 3);
        assert(list_contains(list, "FRFR1PNSXP47CUV6NSXP47CUV6"));
        assert(list_contains(list, "FRFR1PEOGRMLSQTWEOGRMLSQTW"));
        assert(list_contains(list, "FRQOVP5400002FR*QOV*54-00002"));

        defer(free(list));
        defer(free(list->list));
        defer(table_destroy(table));
    }
}

snow_main();
