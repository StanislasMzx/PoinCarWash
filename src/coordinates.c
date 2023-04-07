#define EARTH_RADIUS 6371

#include "coordinates.h"
#include <math.h>

/* computes hav(theta) where theta is given in rad */
double haversine(double theta)
{
    return (1 - cos(theta)) / 2;
}

/* computes the distance between two coordinates in kilometers */
double distance(coordinates *c1, coordinates *c2)
{
    double c1_lat = c1->latitude * M_PI / 180;
    double c2_lat = c2->latitude * M_PI / 180;
    double c1_lon = c1->longitude * M_PI / 180;
    double c2_lon = c2->longitude * M_PI / 180;
    double distance = 2 * EARTH_RADIUS * asin(sqrt(haversine(c2_lat - c1_lat) + cos(c1_lat) * cos(c2_lat) * haversine(c2_lon - c1_lon)));
    return round(distance * 1000) / 1000;
}
