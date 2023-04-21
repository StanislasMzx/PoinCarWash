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

enum _Point_type_t
{
    DEPARTURE,
    ARRIVAL,
    STATION
};
/**
 * @typedef Point_type_t
 * @brief Type of a point
 *
 * @var Point_type_t::DEPARTURE
 * Departure point
 * @var Point_type_t::ARRIVAL
 * Arrival point
 * @var Point_type_t::STATION
 * Station point
 */
typedef enum _Point_type_t Point_type_t;

double haversine(double theta);

double distance(Coordinates_t *c1, Coordinates_t *c2);

Coordinates_t coordinates_input(Point_type_t type);

#endif /* COORDINATES_H */
