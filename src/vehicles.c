#define _GNU_SOURCE
#include "vehicles.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @brief Find a vehicle by name in the vehicle table
 *
 * @param name Name of the vehicle
 * @return Vehicle with NULL values if not found
 */
Vehicle_t vehicle_find_by_name(char *name)
{
    FILE *fp = fopen(VEHICLE_TABLE_PATH, "r");
    assert(fp != NULL);

    const unsigned max_line = 256;
    char line[max_line];
    char *token;
    char *vehicle_name;
    double range;
    unsigned int fast_charge;

    Vehicle_t vehicle = {NULL, 0, 0};

    // Skip the first line
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp))
    {
        token = strtok(line, ",");
        vehicle_name = token;
        token = strtok(NULL, ",");
        range = atoi(token);
        token = strtok(NULL, ",");
        fast_charge = atoi(token);

        if (strcasestr(vehicle_name, name) != NULL)
        {
            vehicle.name = vehicle_name;
            vehicle.range = range;
            vehicle.fast_charge = fast_charge;
            break;
        }
    }

    fclose(fp);

    return vehicle;
}

/**
 * @brief Input a vehicle from stdin
 *
 * @return Vehicle with NULL values if not found
 */
Vehicle_t vehicle_input()
{
    const unsigned MAX_LENGTH = 128;
    char buffer[MAX_LENGTH];
    printf("\33[1;32m");
    printf("Vehicle: ");
    printf("\33[0m");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\r\n")] = '\0';
    Vehicle_t vehicle = vehicle_find_by_name(buffer);
    if (vehicle.name == NULL)
    {
        printf("\33[0;31m");
        printf("Vehicle not found\n");
        printf("\33[0m");
    }
    else
    {
        printf("\33[0;34m");
        printf("Range: ");
        printf("\33[0m");
        printf("%f km\n", vehicle.range);
        printf("\33[0;34m");
        printf("Fast charge: ");
        printf("\33[0m");
        printf("%u km/h\n", vehicle.fast_charge);
    }
    return vehicle;
}
