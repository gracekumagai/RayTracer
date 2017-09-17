#include <stdio.h>
#include "../GeometricTypes/Point3D.h"
#include <time.h>
#include <complex>


double drand48();

int randInt(int min, int max);

double randDouble(double min, double max);

Point3D polarToCartesian2D(double theta, double r);

double radians(double degrees);

static std::complex<double> complexSqrt(const std::complex<double> &z);
static std::complex<double> complexCubeRt(const std::complex<double> &z);

int findRealRoots(std::complex<double> r[4], double realRoots[4]);

int solveQuatric(const double c[5], double r[4]);

int findNumPositiveRoots(double r[4], int n);

int solveQuartic(double coeff[5], double roots[4]);


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