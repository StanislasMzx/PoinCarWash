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
        assert(almosteq(haversine(M_PI_4), (2 - sqrt(2)) / 4));
        assert(almosteq(haversine(M_PI_2), 0.5));
    }
}

describe(test_distance)
{
    it("Conformance test")
    {
        coordinates c1 = {48.66900569983901, 6.155157338272221};
        coordinates c2 = {48.71430027984775, 2.2112903696996775};
        asserteq_dbl(distance(&c1, &c2), 289.495);
        c1 = (coordinates){38.89767470121746, -77.03652996339325};
        c2 = (coordinates){48.869789248005425, 2.3164779677295297};
        asserteq_dbl(distance(&c1, &c2), 6162.471);
    }
}

snow_main();
