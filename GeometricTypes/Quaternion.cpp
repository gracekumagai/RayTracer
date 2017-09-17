#include "Quaternion.h"
#include "../Utilities/Math.h"

Quaternion::Quaternion()
{
	myV = Vector();
	myW = 1.0;
}

Quaternion::Quaternion(double x, double y, double z, double w)
{
	myV = Vector(x, y, z);
	myW = w;
}

Quaternion::Quaternion(Vector v, double w)
{
	myV = v;
	myW = w;
}

Quaternion::Quaternion(const Transform3D &t)
{
	double trace = t.get(0, 0) + t.get(1, 1) + t.get(2, 2);
	if (trace > 0)
	{
		double s = sqrt(trace + 1.0);

		myW = s / 2.0;
		s = 0.5 / s;

		myV.x = (t.get(2, 1) - t.get(1, 2)) * s;
		myV.y = (t.get(0, 2) - t.get(2, 0)) * s;
		myV.z = (t.get(1, 0) - t.get(0, 1)) * s;
	}
	else
	{
		if (t.get(0, 0) > t.get(1, 1) && t.get(0, 0) > t.get(2, 2)) // t[0][0] max
		{
			double s = 2.0 * sqrt(1.0 + t.get(0, 0) - t.get(1, 1) - t.get(2, 2));
			myW = (t.get(2, 1) - t.get(1, 2)) / s;
			myV.x = 0.25 * s;
			myV.y = (t.get(0, 1) + t.get(1, 0)) / s;
			myV.z = (t.get(0, 2) + t.get(2, 0)) / s;
		}
		else if (t.get(1, 1) > t.get(2, 2))							// t[1][1] max
		{
			double s = 2.0 * sqrt(1.0 + t.get(1, 1) - t.get(2, 2) - t.get(0, 0));
			myW = (t.get(0, 2) - t.get(2, 0)) / s;
			myV.x = (t.get(0, 1) + t.get(1, 0)) / s;
			myV.y = 0.25 * s;
			myV.z = (t.get(1, 2) + t.get(2, 1)) / s;
		}
		else														// t[2][2] max
		{
			double s = 2.0 * sqrt(1.0 + t.get(2, 2) - t.get(1, 1) - t.get(0, 0));
			myW = (t.get(1, 0) - t.get(0, 1)) / s;
			myV.x = (t.get(0, 2) + t.get(2, 0)) / s;
			myV.y = (t.get(1, 2) + t.get(2, 1)) / s;
			myV.z = 0.25 * s;
		}
	}
}

Quaternion &Quaternion::operator+=(const Quaternion &q)
{
	this->myV += q.myV;
	this->myW += q.myW;

	return *this;

}

Quaternion operator+(const Quaternion &q1, const Quaternion &q2)
{
	Quaternion ret = q1;
	return ret += q2;
}

Quaternion &Quaternion::operator-=(const Quaternion &q)
{
	this->myV -= q.myV;
	this->myW -= q.myW;
	return *this;
}

Quaternion operator-(const Quaternion &q1, const Quaternion &q2)
{
	Quaternion ret = q1;
	return ret -= q2;
}

Quaternion Quaternion::operator*(double scale) const
{
	return Quaternion(Vector(myV * scale), myW * scale);
}

double Quaternion::dot(const Quaternion &other) const {
	return this->myV.dot(other.myV) + this->myW*other.myW;
}

Quaternion Quaternion::normalized() const
{
	return (*this) * (1.0 / (sqrt((*this).dot(*this))));
}

Transform3D Quaternion::findMatrix() const
{
	double qxx = myV.x * myV.x, qyy = myV.y * myV.y, qzz = myV.z * myV.z;
	double qxy = myV.x * myV.y, qxz = myV.x * myV.z, qyz = myV.y * myV.z;
	double qxw = myW * myV.x, qyw = myW * myV.y, qzw = myV.z, myW;

	return Transform3D(1 - 2 * (qyy + qzz), 2 * (qxy + qzw), 2 * (qxz - qyw), 0.0,
					   2 * (qxy - qzw), 1 - 2 * (qxx + qzz), 2 * (qyz + qxw), 0.0,
					   2 * (qxz + qyw), 2 * (qyz - qxw), 1 - 2 * (qxx + qyy), 0.0,
					   0.0, 0.0, 0.0, 1.0).transpose();
}

/* Method Quaternion Functions */
Quaternion Slerp(const Quaternion &q1, const Quaternion &q2, double t)
{
	// slerp(q1, q2, t) = (q1 sin((1-t)*theta) + q2 sin(t*theta)) / sin(theta)
	double cosT = q1.dot(q2);
	if (cosT > 0.9995)
	{
		return (q1 * (1.0 - t) + q2 * t).normalized();
	}
	else
	{
		double theta = acos(Utility::Clamp(cosT, -1.0, 1.0));
		double thetaP = theta * t;
		Quaternion qPerp = (q2 - q1 * cosT).normalized();
		return q1 * cos(thetaP) + qPerp * sin(thetaP);
	}
}
