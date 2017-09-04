#ifndef PerlinNoise_h
#define PerlinNoise_h

#include <stdio.h>
#include "../GeometricTypes/Point3D.h"

// forward declare the object type to avoid circular dependencies
class Object3D;

class PerlinNoise {
	std::vector<int> p;

public:
	PerlinNoise();
   
	PerlinNoise(unsigned int seed);

	double noise(double x, double y, double z);
private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};

#endif
