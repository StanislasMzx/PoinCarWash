#ifndef COORDINATES_H
#define COORDINATES_H

struct _coordinates
{
    double latitude;
    double longitude;
};

typedef struct _coordinates coordinates;

double haversine(double theta);
double distance(coordinates *c1, coordinates *c2);

#endif /* COORDINATES_H */
