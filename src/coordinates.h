#ifndef COORDINATES_H
#define COORDINATES_H

struct _Coordinates
{
    double latitude;
    double longitude;
};
/**
 * @typedef Coordinates
 * @brief Coordinates of a point on the Earth
 *
 * @var Coordinates::latitude
 * Latitude of the point
 * @var Coordinates::longitude
 * Longitude of the point
 */
typedef struct _Coordinates Coordinates;

double haversine(double theta);
double distance(Coordinates *c1, Coordinates *c2);

#endif /* COORDINATES_H */
