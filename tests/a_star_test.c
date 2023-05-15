#define SNOW_ENABLED

#include "snow.h"
#include "../src/a_star.h"

/*
describe(test_a_star_next_stations)
{
    it("Conformance test")
    {
        Table_t *table = load_stations("../data/raw/schema-stations-test.csv");

        Vehicle_t *one_vehicle = malloc(sizeof(Vehicle_t));

        *one_vehicle = vehicle_find_by_name("Tesla Model 3");

        char *end_key = "FROTHPOTHR18111FR*SOD*S*OTHR*181*1*_*_";

        char *start_key = "FROTHPOTHR17951FR*SOD*S*OTHR*179*5*_*_";

        Station_t *end = table_get(table, end_key);
        assert(end != NULL);

        Station_t *start = table_get(table, start_key);
        assert(start != NULL);

        Heap_t *one_heap = heap_empty();

        a_star_next_stations(table, start, start_key, one_vehicle->range, &one_heap, 0, end);

        State_t *state;
        Station_t *station;
        while (!heap_is_empty(one_heap))
        {
            state = heap_pop(&one_heap, heap_height(one_heap));
            station = table_get(table, state->id_station);
            assert(distance(station->coordinates, start->coordinates) <= one_vehicle->range);
            assert(distance(station->coordinates, start->coordinates) != 0);
            state_destroy(state);
        }

        defer(heap_destroy(one_heap));
        defer(table_destroy(table));
        defer(free(one_vehicle));
    }
}

describe(test_a_star)
{
    it("Conformance test")
    {
        Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");

        Vehicle_t *one_vehicle = malloc(sizeof(Vehicle_t));

        *one_vehicle = vehicle_find_by_name("Tesla Model 3");

        char *end_key = "FRFR1PRYXKLFRYXKLF";

        char *start_key = "FRFR1PQXX0ZUA3NLQXX0ZUA3NL";

        List_t *one_list = a_star_list(table, start_key, end_key, one_vehicle, 0, 10000000);

        print_a_star(table, one_list, 0, 10000000);

        asserteq_str("FRFR1PQXX0ZUA3NLQXX0ZUA3NL", one_list->list[0].key);
        asserteq_str("FRTDAPAVTD1411FR*SOD*S*AVTD*14*1*_*_", one_list->list[1].key);
        asserteq_str("FRFR1PRYXKLFRYXKLF", one_list->list[2].key);

        defer(list_destroy(one_list));
        defer(table_destroy(table));
        defer(free(one_vehicle));
    }
}


describe(test_a_star_min_power)
{
    it("Conformance test")
    {
        Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");

        Vehicle_t *one_vehicle = malloc(sizeof(Vehicle_t));

        *one_vehicle = vehicle_find_by_name("Tesla Model 3");

        double power_min = 30;

        double time_in_station_max = 1000000;

        one_vehicle->range = 100/(100-power_min)*one_vehicle->range;

        char *end_key = "FRFR1PRYXKLFRYXKLF";

        char *start_key = "FRFR1PQXX0ZUA3NLQXX0ZUA3NL";

        List_t *one_list = a_star_list(table, start_key, end_key, one_vehicle, power_min, time_in_station_max);

        print_a_star(table, one_list, power_min, time_in_station_max);

        for (int i=1; i<one_list->length; i++){
            double dist = distance(one_list->list[i-1].value->coordinates, one_list->list[i].value->coordinates);
            assert(one_vehicle->range-power_min/100*one_vehicle->range >= dist);
        }

        asserteq_str("FRFR1PQXX0ZUA3NLQXX0ZUA3NL", one_list->list[0].key);
        asserteq_str("FRTDAPAVTD1411FR*SOD*S*AVTD*14*1*_*_", one_list->list[1].key);
        asserteq_str("FRFR1PRYXKLFRYXKLF", one_list->list[2].key);

        defer(list_destroy(one_list));
        defer(table_destroy(table));
        defer(free(one_vehicle));
    }
}*/

describe(test_a_star_time_max)
{
    it("Conformance test")
    {
        Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");

        Vehicle_t *one_vehicle = malloc(sizeof(Vehicle_t));

        *one_vehicle = vehicle_find_by_name("Tesla Model 3");

        double power_min = 0;

        double time_in_station_max = 30;

        one_vehicle->range = 100/(100-power_min)*one_vehicle->range;

        char *end_key = "FRFR1PRYXKLFRYXKLF";

        char *start_key = "FRFR1PQXX0ZUA3NLQXX0ZUA3NL";

        List_t *one_list = a_star_list(table, start_key, end_key, one_vehicle, power_min, time_in_station_max);

        print_a_star(table, one_list, power_min, time_in_station_max);

        for (int i=1; i<one_list->length; i++){
            double dist = distance(one_list->list[i-1].value->coordinates, one_list->list[i].value->coordinates);
            double range_min = MIN(one_vehicle->fast_charge*time_in_station_max/60, time_in_station_max/60*one_list->list[i-1].value->power);
            assert(range_min >= dist);
        }

        asserteq_str("FRFR1PQXX0ZUA3NLQXX0ZUA3NL", one_list->list[0].key);
        asserteq_str("FRFR1PGUKJEMGUKJEM", one_list->list[1].key);
        //asserteq_str("FRS33PMB3313412FR*SOD*S*MB33*134*1*_*_", one_list->list[2].key);
        //asserteq_str("FRS16PMB164712FR*SOD*S*MB16*47*1*_*_", one_list->list[3].key);
        //asserteq_str("FRFR1PRYXKLFRYXKLF", one_list->list[4].key);

        defer(list_destroy(one_list));
        defer(table_destroy(table));
        defer(free(one_vehicle));
    }
}

snow_main();
