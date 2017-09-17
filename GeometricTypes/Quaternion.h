#ifndef Quaternion_h
#define Quaternion_h

#include "Point3D.h"
#include "Transform3D.h"

class Quaternion
{
public:
	Vector myV;
	double myW;

	Quaternion();
	Quaternion(double x, double y, double z, double w);
	Quaternion(Vector v, double w);
	Quaternion(const Transform3D &t);

	friend Quaternion operator+(const Quaternion &q1, const Quaternion &q2);
	Quaternion &operator+=(const Quaternion &q);
	friend Quaternion operator-(const Quaternion &q1, const Quaternion &q2);
	Quaternion &operator-=(const Quaternion &q);
	Quaternion operator*(double scale) const;

	double dot(const Quaternion &other) const;

	Quaternion normalized() const;

	Transform3D findMatrix() const;
};

Quaternion Slerp(const Quaternion &q1, const Quaternion &q2, double t);

#endif
