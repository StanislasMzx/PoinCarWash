#define SNOW_ENABLED

#include "snow.h"
#include "../src/monitoring.h"
#include <math.h>

describe(test_vehicle_position)
{
    it("Conformance test")
    {
        Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");

        Nominatim_t *startNomin = nominatim_fetch("nancy");
        Nominatim_t *endNomin = nominatim_fetch("paris");
        Vehicle_t vehicle = vehicle_find_by_name("Tesla Model 3");

        Journey_output_t output = compute_journey(table, startNomin, endNomin, "Tesla Model 3");

        asserteq_str(vehicle_position(&vehicle, &output, 0), "on the road");
        asserteq_str(vehicle_position(&vehicle, &output, 1), "on the road");
        asserteq_str(table_get(table, vehicle_position(&vehicle, &output, 15))->name, "Parking Acelec");
        asserteq_str(table_get(table, vehicle_position(&vehicle, &output, 16))->name, "Parking Acelec");
        asserteq_str(vehicle_position(&vehicle, &output, 17), "on the road");

        table_destroy(table);
        list_destroy(output.journey);
        nominatim_destroy(startNomin);
        nominatim_destroy(endNomin);
    }
}

snow_main();
