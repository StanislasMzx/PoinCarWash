#ifndef VEHICLES_H
#define VEHICLES_H

#define VEHICLE_TABLE_PATH "../data/raw/vehicle_info.csv"

struct _Vehicle_t
{
    char *name;
    double range;
    unsigned int fast_charge;
};
/**
 * @typedef Vehicle_t
 * @brief Vehicle type
 *
 * @var Vehicle::name
 * Name of the vehicle
 * @var Vehicle::range
 * Range of the vehicle in km
 * @var Vehicle::fast_charge
 * Fast charge speed of the vehicle in km/h
 */
typedef struct _Vehicle_t Vehicle_t;

void vehicle_destroy(Vehicle_t *vehicle);

Vehicle_t *vehicle_find_by_name(char *name);

Vehicle_t *vehicle_input();

#endif /* VEHICLES_H */
