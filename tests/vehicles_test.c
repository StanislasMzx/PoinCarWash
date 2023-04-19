#define SNOW_ENABLED

#include "snow.h"
#include "../src/vehicles.h"

describe(test_vehicle_find_by_name)
{
    it("Conformance test")
    {
        Vehicle_t vehicle = vehicle_find_by_name("Tesla Model 3 Performance");
        asserteq_str(vehicle.name, "Tesla Model 3 Performance");
        asserteq_int(vehicle.range, 460);
        asserteq_int(vehicle.fast_charge, 710);

        vehicle = vehicle_find_by_name("Mercedes EQE 350+");
        asserteq_str(vehicle.name, "Mercedes EQE 350+");
        asserteq_int(vehicle.range, 525);
        asserteq_int(vehicle.fast_charge, 780);
    }
    it("Error test")
    {
        Vehicle_t vehicle = vehicle_find_by_name("Not a vehicle");
        assert(vehicle.name == NULL);
        asserteq_int(vehicle.range, 0);
        asserteq_int(vehicle.fast_charge, 0);
    }
}

// describe(test_vehicle_input)
// {
//     it("Conformance test")
//     {
//         Vehicle_t vehicle = vehicle_input();
//         assert(vehicle.name != NULL);
//         asserteq_str(vehicle.name, "Volvo C40 Recharge Twin Motor");
//         asserteq_int(vehicle.range, 390);
//         asserteq_int(vehicle.fast_charge, 600);
//     }
// }

snow_main();
