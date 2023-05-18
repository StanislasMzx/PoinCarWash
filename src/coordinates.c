#define EARTH_RADIUS 6371
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "coordinates.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Computes the haversine of an angle
 *
 * @param theta Angle in radians
 * @return double Haversine of the angle
 */
double haversine(double theta)
{
    return (1 - cos(theta)) / 2;
}

/**
 * @brief Computes the distance between two coordinates in kilometers
 *
 * @param c1 First coordinate
 * @param c2 Second coordinate
 * @return double Distance between the two coordinates in kilometers with a precision of 3 decimals
 */
double distance(Coordinates_t *c1, Coordinates_t *c2)
{
    double c1_lat = c1->latitude * M_PI / 180;
    double c2_lat = c2->latitude * M_PI / 180;
    double c1_lon = c1->longitude * M_PI / 180;
    double c2_lon = c2->longitude * M_PI / 180;
    double distance = 2 * EARTH_RADIUS * asin(sqrt(haversine(c2_lat - c1_lat) + cos(c1_lat) * cos(c2_lat) * haversine(c2_lon - c1_lon)));
    return round(distance * 1000) / 1000;
}

/**
 * @brief Input coordinates from stdin
 *
 * @param type Type of the point
 * @return Coordinates_t Coordinates of the point
 */
Coordinates_t coordinates_input(Point_type_t type)
{
    const unsigned MAX_LENGTH = 128;
    char buffer[MAX_LENGTH];
    Coordinates_t coordinates;
    printf("\33[1;32m");
    printf("%s:\n", type == DEPARTURE ? "Departure point" : "Arrival point");
    printf("\33[0;34m");
    printf("Latitude: ");
    printf("\33[0m");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\r\n")] = '\0';
    coordinates.latitude = atof(buffer);
    printf("\33[0;34m");
    printf("Longitude: ");
    printf("\33[0m");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\r\n")] = '\0';
    coordinates.longitude = atof(buffer);
    return coordinates;
}

void coordinates_print(Coordinates_t *c1)
{
    printf("{longitude : %f, latitude : %f}", c1->longitude, c1->latitude);
}
