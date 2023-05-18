#define SNOW_ENABLED

#include "snow.h"
#include "../src/a_star.h"

describe(test_a_star_next_stations)
{
    it("Conformance test")
    {
        Table_t *table = load_stations("../data/raw/schema-stations-test.csv");

        Vehicle_t *one_vehicle = vehicle_find_by_name("Tesla Model 3");

        char *end_key = "FROTHPOTHR18111FR*SOD*S*OTHR*181*1*_*_";

        char *start_key = "FROTHPOTHR17951FR*SOD*S*OTHR*179*5*_*_";

        Station_t *end = table_get(table, end_key);
        assert(end != NULL);

        Station_t *start = table_get(table, start_key);
        assert(start != NULL);

        Heap_t *one_heap = heap_empty();

        a_star_next_stations(table, start_key, one_vehicle->range, &one_heap, 0, end);

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
        defer(vehicle_destroy(one_vehicle));
    }
}

describe(test_a_star)
{
    it("Conformance test")
    {
        Table_t *table = load_stations(STATION_TABLE_PATH);

        Vehicle_t *one_vehicle = vehicle_find_by_name("Tesla Model 3");

        char *end_key = "FRFR1PRYXKLFRYXKLF";

        char *start_key = "FRFR1PQXX0ZUA3NLQXX0ZUA3NL";

        List_t *one_list = a_star_list(table, start_key, end_key, one_vehicle);

        print_a_star(table, one_list, one_vehicle->range);

        asserteq_str("FRFR1PQXX0ZUA3NLQXX0ZUA3NL", one_list->list[0].key);
        asserteq_str("FRTDAPAVTD1411FR*SOD*S*AVTD*14*1*_*_", one_list->list[1].key);
        asserteq_str("FRFR1PRYXKLFRYXKLF", one_list->list[2].key);

        defer(list_destroy(one_list));
        defer(table_destroy(table));
        defer(vehicle_destroy(one_vehicle));
    }
}

snow_main();
