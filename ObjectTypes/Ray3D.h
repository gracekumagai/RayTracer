#ifndef Ray3D_h
#define Ray3D_h

#include <stdio.h>
#include "../GeometricTypes/Point3D.h"

// forward declare the object type to avoid circular dependencies
class Object3D;

class Ray3D {
    const double biasAmount = 1e-6;
    
public:
    Point3D myOrigin;
    Point3D myDirection;

	mutable double myMaxT;
	double myTime;
    
	Ray3D() :  myMaxT(INFINITY), myTime(0.0) {}
    Ray3D(Point3D origin, Point3D direction, double maxT = INFINITY, double time = 0.0);
    Ray3D(const Ray3D &ray);
    Ray3D& operator=(const Ray3D &ray);
    
    // Function for returning the position along the ray
    // for a given lambda.
    Point3D rayPosition(double lambda) const;
    
    Ray3D bias(const Point3D &normal) const;
    
    Ray3D offset(Point3D amount) const;
    
    Ray3D normalized() const;
};


class RayDifferential : public Ray3D
{
public:
	bool myHasDifferentials;
	Point3D myXOrigin, myYOrigin, myXDirection, myYDirection;

	RayDifferential() { myHasDifferentials = false; }
	RayDifferential(Point3D origin, Point3D direction, double maxT = INFINITY, double time = 0.0);
	RayDifferential(const Ray3D &ray);

	void scaleDifferentials(double scale);
};
#endif
