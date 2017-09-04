#include <stdio.h>
#include "../GeometricTypes/Point3D.h"
#include <time.h>

#define Infinity std::numeric_limits<Float>::infinity()

double drand48();

int randInt(int min, int max);

double randDouble(double min, double max);

Point3D polarToCartesian2D(double theta, double r);

namespace Utility {

	// Global Inline Functions
	template <typename T, typename U, typename V>
	inline T Clamp(T val, U low, V high) {
		if (val < low)
			return low;
		else if (val > high)
			return high;
		else
			return val;
	}
}