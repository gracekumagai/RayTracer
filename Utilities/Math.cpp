#include "Math.h"
#include <stdlib.h> 

#define PI 3.14159265354

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

double radians(double degrees)
{
	return PI * degrees / 180.0;
}

std::complex<double> complexSqrt(const std::complex<double> &z)
{
	return pow(z, 1.0 / 2.0);
}

std::complex<double> complexCubeRt(const std::complex<double> &z)
{
	return pow(z, 1.0 / 3.0);
}

int findRealRoots(std::complex<double> r[4], double realRoots[4])
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (r[i].imag() == 0.0)
		{
			realRoots[i] = r[i].real();
			count++;
		}
		else
		{
			realRoots[i] = INFINITY;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (realRoots[j] > realRoots[j + 1])
			{
				double tmp = realRoots[j];
				realRoots[j] = realRoots[j + 1];
				realRoots[j + 1] = tmp;
			}
		}
	}
	return count;
}

int solveQuatric(const double c[5], double r[4])
{
	// From https://github.com/sidneycadot/quartic
	// Solve for roots of quatric equation
	// where, Ax^4 + Bx^3 + Cx^2 + Dx + E = 0
	// where, coefficients = {A, B, C, D, E}
	const std::complex<double> A = { c[0], 0.0 };
	const std::complex<double> B = { c[1] / c[0], 0.0 };
	const std::complex<double> C = { c[2] / c[0], 0.0 };
	const std::complex<double> D = { c[3] / c[0], 0.0 };
	const std::complex<double> E = { c[4] / c[0], 0.0 };

	const std::complex<double> Q1 = C * C - 3. * B * D + 12. * E;
	const std::complex<double> Q2 = 2. * C * C * C - 9. * B * C * D + 27. * D * D + 27. * B * B * E - 72. * C * E;
	const std::complex<double> Q3 = 8. * B * C - 16. * D - 2. * B * B * B;
	const std::complex<double> Q4 = 3. * B * B - 8. * C;

	const std::complex<double> Q5 = complexCubeRt(Q2 / 2. + complexSqrt(Q2 * Q2 / 4. - Q1 * Q1 * Q1));
	const std::complex<double> Q6 = (Q1 / Q5 + Q5) / 3.;
	const std::complex<double> Q7 = 2. * complexSqrt(Q4 / 12. + Q6);

	std::complex<double> roots[4];

	roots[0] = (-B - Q7 - complexSqrt(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7)) / 4.;
	roots[1] = (-B - Q7 + complexSqrt(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7)) / 4.;
	roots[2] = (-B + Q7 - complexSqrt(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7)) / 4.;
	roots[3] = (-B + Q7 + complexSqrt(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7)) / 4.;

	return findRealRoots(roots, r);
}

int findNumPositiveRoots(double r[4], int n)
{
	if (n == 0)
		return 0;

	int count = 0;
	for (int i = 0; i < n; i++)
	{
		if (r[i] >= 0)
		{
			count++;
		}
	}
	return count;
}

void solve_cubic_equation
(double  c3, double  c2, double  c1, double c0,
	double& x1, double& x2, double& x3, int& nr)
{
	double a2 = c2 / c3;
	double a1 = c1 / c3;
	double a0 = c0 / c3;

	double q = a1 / 3.0 - a2*a2 / 9.0;
	double r = (a1*a2 - 3.0*a0) / 6.0 - a2*a2*a2 / 27.0;
	double delta = q*q*q + r*r;

	if (delta>0.0) {
		double s1 = r + sqrt(delta);
		s1 = (s1 >= 0.0) ? pow(s1, 1. / 3.) : -pow(-s1, 1. / 3.);

		double s2 = r - sqrt(delta);
		s2 = (s2 >= 0.0) ? pow(s2, 1. / 3.) : -pow(-s2, 1. / 3.);

		x1 = (s1 + s2) - a2 / 3.0;
		x2 = x3 = -0.5 * (s1 + s2) - a2 / 3.0;

		nr = 1;
	}
	else if (delta < 0.0) {
		double theta = acos(r / sqrt(-q*q*q)) / 3.0;
		double costh = cos(theta);
		double sinth = sin(theta);
		double sq = sqrt(-q);

		x1 = 2.0*sq*costh - a2 / 3.0;
		x2 = -sq*costh - a2 / 3.0 - sqrt(3.) * sq * sinth;
		x3 = -sq*costh - a2 / 3.0 + sqrt(3.) * sq * sinth;

		nr = 3;
	}
	else {
		double s = (r >= 0.0) ? pow(r, 1. / 3.) : -pow(-r, 1. / 3.);
		x1 = 2.0*s - a2 / 3.0;
		x2 = x3 = -s - a2 / 3.0;
		nr = 3;
	}
}

int solveQuartic(double coeff[5], double roots[4])
{
	double r1, r2, r3, r4;
	int nr12, nr34;

	double a3 = coeff[1] / coeff[0];
	double a2 = coeff[2] / coeff[0];
	double a1 = coeff[3] / coeff[0];
	double a0 = coeff[4] / coeff[0];

	double au2 = -a2;
	double au1 = (a1*a3 - 4.0*a0);
	double au0 = 4.0*a0*a2 - a1*a1 - a0*a3*a3;

	double x1, x2, x3;
	int nr;
	solve_cubic_equation(1.0, au2, au1, au0, x1, x2, x3, nr);

	double u1;
	if (nr == 1) {
		u1 = x1;
	}
	else {
		u1 = (x1>x3) ? x1 : x3;
	}

	double R2 = 0.25*a3*a3 + u1 - a2;
	double R = (R2>0.0) ? sqrt(R2) : 0.0;

	double D2, E2;
	if (R != 0.0) {
		double foo1 = 0.75*a3*a3 - R2 - 2.0*a2;
		double foo2 = 0.25*(4.0*a3*a2 - 8.0*a1 - a3*a3*a3) / R;
		D2 = foo1 + foo2;
		E2 = foo1 - foo2;
	}
	else {
		double foo1 = 0.75*a3*a3 - 2.0*a2;
		double foo2 = 2.0 * sqrt(u1*u1 - 4.0*a0);
		D2 = foo1 + foo2;
		E2 = foo1 - foo2;
	}

	if (D2 >= 0.0) {
		double D = sqrt(D2);
		r1 = -0.25*a3 + 0.5*R - 0.5*D;
		r2 = -0.25*a3 + 0.5*R + 0.5*D;
		nr12 = 2;
	}
	else {
		r1 = r2 = -0.25*a3 + 0.5*R;
		nr12 = 0;
	}

	if (E2 >= 0.0) {
		double E = sqrt(E2);
		r3 = -0.25*a3 - 0.5*R - 0.5*E;
		r4 = -0.25*a3 - 0.5*R + 0.5*E;
		nr34 = 2;
	}
	else {
		r3 = r4 = -0.25*a3 - 0.5*R;
		nr34 = 0;
	}

	int i = 0;
	if (nr12 != 0) {
		roots[i++] = r1;
		roots[i++] = r2;
	}
	if (nr34 != 0) {
		roots[i++] = r3;
		roots[i++] = r4;
	}

	// Sort in ascending order
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (roots[j] > roots[j + 1])
			{
				double tmp = roots[j];
				roots[j] = roots[j + 1];
				roots[j + 1] = tmp;
			}
		}
	}

	return nr12 + nr34;
}

