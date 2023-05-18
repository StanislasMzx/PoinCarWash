#define SNOW_ENABLED

#include "snow.h"
#include "../src/monitoring.h"
#include <math.h>

// describe(test_vehicle_position)
// {
//     it("Conformance test")
//     {
//         Table_t *table = load_stations(STATION_TABLE_PATH);

//         Nominatim_t *startNomin = nominatim_fetch("nancy");
//         Nominatim_t *endNomin = nominatim_fetch("paris");
//         Vehicle_t vehicle = vehicle_find_by_name("Tesla Model 3");

//         Trip_output_t output = compute_trip(table, startNomin, endNomin, "Tesla Model 3");

//         asserteq_str(vehicle_position(&vehicle, &output, 0), "on the road");
//         asserteq_str(vehicle_position(&vehicle, &output, 1), "on the road");
//         asserteq_str(table_get(table, vehicle_position(&vehicle, &output, 15))->name, "Parking Acelec");
//         asserteq_str(table_get(table, vehicle_position(&vehicle, &output, 16))->name, "Parking Acelec");
//         asserteq_str(vehicle_position(&vehicle, &output, 17), "on the road");

//         table_destroy(table);
//         list_destroy(output.trip);
//         nominatim_destroy(startNomin);
//         nominatim_destroy(endNomin);
//     }
// }

describe(test_load_network)
{
    it("Conformance test")
    {
        Table_t *table = load_stations(STATION_TABLE_PATH);
        Trip_output_t *network = load_network(table, "../data/input/network1.csv", 2);
        asserteq_str(network[0].trip->list[1].value->name, "Val de Meuse, Parking Intermarché");
        asserteq_str(network[0].trip->list[2].value->name, "Langres, Place Bel Air");
        asserteq_str(network[0].trip->list[3].value->name, "SIEG63 - ePremium - Montpeyroux - Parking Entree");
        asserteq_str(network[1].trip->list[1].value->name, "Echirolles, 10 Cours Jean Jaurès");
        asserteq_str(network[1].trip->list[2].value->name, "Chaintre, place du luminaire");
        asserteq_str(network[1].trip->list[3].value->name, "RELAIS DE LA COULINE");
        for (int i = 0; i < 2; i++)
        {
            list_destroy(network[i].trip);
        }
        free(network);
        table_destroy(table);
    }
}

snow_main();
