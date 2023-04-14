#include "vehicles.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @brief Find a vehicle by name in /data/raw/vehicle_info.csv
 *
 * @param name Name of the vehicle
 * @return Vehicle with NULL values if not found
 */
Vehicle_t vehicle_find_by_name(char *name)
{
    FILE *fp = fopen("../data/raw/vehicle_info.csv", "r");
    assert(fp != NULL);

    const unsigned max_line = 256;
    char line[max_line];
    char *token;
    char *vehicle_name;
    unsigned int range;
    unsigned int fast_charge;

    Vehicle_t vehicle = {NULL, 0, 0};

    while (fgets(line, sizeof(line), fp))
    {
        token = strtok(line, ",");
        vehicle_name = token;
        token = strtok(NULL, ",");
        range = atoi(token);
        token = strtok(NULL, ",");
        fast_charge = atoi(token);

        if (strcmp(vehicle_name, name) == 0)
        {
            vehicle.name = name;
            vehicle.range = range;
            vehicle.fast_charge = fast_charge;
            break;
        }
    }

    fclose(fp);
    return vehicle;
}
