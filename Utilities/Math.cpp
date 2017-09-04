#include "Math.h"
#include <stdlib.h> 

double drand48()
{
	return (double)rand() / RAND_MAX;
}

int randInt(int min, int max)
{
	return min + rand() % (max - min + 1);
}

double randDouble(double min, double max)
{
	double r = drand48();
	return min + r * (max - min);
}

Point3D polarToCartesian2D(double theta, double r)
{
	return Point3D(r*cos(theta), r*sin(theta), 0.0, false);
}