#define SNOW_ENABLED

#include "snow.h"
#include "../src/vehicles.h"

describe(test_vehicle_find_by_name)
{
    it("Conformance test")
    {
        Vehicle_t *vehicle1 = vehicle_find_by_name("Tesla Model 3 Performance");
        asserteq_str(vehicle1->name, "Tesla Model 3 Performance");
        asserteq_int(vehicle1->range, 460);
        asserteq_int(vehicle1->fast_charge, 710);

        Vehicle_t *vehicle2 = vehicle_find_by_name("Mercedes EQE 350+");
        asserteq_str(vehicle2->name, "Mercedes EQE 350+");
        asserteq_int(vehicle2->range, 525);
        asserteq_int(vehicle2->fast_charge, 780);

        Vehicle_t *vehicle3 = vehicle_find_by_name("zoe");
        asserteq_str(vehicle3->name, "Renault Zoe ZE50 R110");
        asserteq_int(vehicle3->range, 315);
        asserteq_int(vehicle3->fast_charge, 230);

        vehicle_destroy(vehicle1);
        vehicle_destroy(vehicle2);
        vehicle_destroy(vehicle3);
    }
    it("Error test")
    {
        Vehicle_t *vehicle = vehicle_find_by_name("Not a vehicle");
        assert(vehicle->name == NULL);
        asserteq_int(vehicle->range, 0);
        asserteq_int(vehicle->fast_charge, 0);

        vehicle_destroy(vehicle);
    }
}

// describe(test_vehicle_input)
// {
//     it("Conformance test")
//     {
//         Vehicle_t *vehicle = vehicle_input();
//         assert(vehicle->name != NULL);
//         asserteq_str(vehicle->name, "Volvo C40 Recharge Twin Motor");
//         asserteq_int(vehicle->range, 390);
//         asserteq_int(vehicle->fast_charge, 600);
//     }
// }

snow_main();
