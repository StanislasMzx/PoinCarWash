#ifndef COORDINATES_H
#define COORDINATES_H

struct _Coordinates_t
{
    double latitude;
    double longitude;
};
/**
 * @typedef Coordinates_t
 * @brief Coordinates of a point on the Earth
 *
 * @var Coordinates::latitude
 * Latitude of the point
 * @var Coordinates::longitude
 * Longitude of the point
 */
typedef struct _Coordinates_t Coordinates_t;

double haversine(double theta);
double distance(Coordinates_t *c1, Coordinates_t *c2);

#endif /* COORDINATES_H */
