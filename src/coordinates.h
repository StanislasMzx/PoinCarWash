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

/**
 * @brief Computes the haversine of an angle
 *
 * @param theta Angle in radians
 * @return double Haversine of the angle
 */
double haversine(double theta);

/**
 * @brief Computes the distance between two coordinates in kilometers
 *
 * @param c1 First coordinate
 * @param c2 Second coordinate
 * @return double Distance between the two coordinates in kilometers with a precision of 3 decimals
 */
double distance(Coordinates_t *c1, Coordinates_t *c2);

/**
 * @brief Input coordinates from stdin
 *
 * @param type Type of the point
 * @return Coordinates_t Coordinates of the point
 */
Coordinates_t coordinates_input(Point_type_t type);

/**
 * @brief Print some coordinates
 *
 * @param c1 Coordinates to print
 */
void coordinates_print(Coordinates_t *c1);

#endif /* COORDINATES_H */
