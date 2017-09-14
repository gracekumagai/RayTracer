#include <stdio.h>
#include "../GeometricTypes/Point3D.h"
#include <time.h>

double drand48();

int randInt(int min, int max);

double randDouble(double min, double max);

Point3D polarToCartesian2D(double theta, double r);

namespace Utility {

	// Global Inline Functions
	template <typename T, typename U, typename V>
	inline T Clamp(T val, U low, V high) 
	{
		if (val < low)
			return low;
		else if (val > high)
			return high;
		else
			return val;
	}

	template <typename T>
	inline T max(T a, T b)
	{
		if (a < b)
			return b;
		return a;
	}

	template <typename T>
	inline T min(T a, T b)
	{
		if (a < b)
			return a;
		return b;
	}

	template <typename T>
	inline T Lerp(double t, T s1, T s2)
	{
		return (1.0 - t) * s1 + t * s2;
	}
}