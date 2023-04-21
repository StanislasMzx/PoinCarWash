#define SNOW_ENABLED
#define EARTH_RADIUS 6371
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "snow.h"
#include "../src/coordinates.h"
#include <math.h>
#include <float.h>
#include <stdbool.h>

bool almosteq(double a, double b);

bool almosteq(double a, double b)
{
    return (fabs(a - b) < DBL_EPSILON);
}

describe(test_haversine)
{
    it("Conformance test")
    {
        assert(almosteq(haversine(0), 0));
        assert(almosteq(haversine(M_PI), 1));
        assert(almosteq(haversine(M_PI / 4), (2 - sqrt(2)) / 4));
        assert(almosteq(haversine(M_PI / 2), 0.5));
    }
}

describe(test_distance)
{
    it("Conformance test")
    {
        Coordinates_t c1 = {48.66900569983901, 6.155157338272221};
        Coordinates_t c2 = {48.71430027984775, 2.2112903696996775};
        asserteq_dbl(distance(&c1, &c2), 289.495);
        c1 = (Coordinates_t){38.89767470121746, -77.03652996339325};
        c2 = (Coordinates_t){48.869789248005425, 2.3164779677295297};
        asserteq_dbl(distance(&c1, &c2), 6162.471);
    }
}

// describe(test_coordinates_input)
// {
//     it("Conformance test")
//     {
//         Coordinates_t c = coordinates_input(DEPARTURE);
//         asserteq_dbl(c.latitude, 48.66900569983901);
//         asserteq_dbl(c.longitude, 6.155157338272221);
//         c = coordinates_input(ARRIVAL);
//         asserteq_dbl(c.latitude, 48.71430027984775);
//         asserteq_dbl(c.longitude, 2.2112903696996775);
//     }
// }

snow_main();
