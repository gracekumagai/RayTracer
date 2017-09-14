#include "Ray3D.h"

/* RAY */
Ray3D::Ray3D(Point3D origin, Point3D direction, double maxT, double time) {
    this->myOrigin = origin;
    this->myDirection = direction;
}

Ray3D::Ray3D(const Ray3D &ray) {
    myOrigin = ray.myOrigin;
    myDirection = ray.myDirection;
	myMaxT = ray.myMaxT;
	myTime = ray.myTime;
}

Ray3D& Ray3D::operator=(const Ray3D &ray) {
    myOrigin = ray.myOrigin;
    myDirection = ray.myDirection;
    return *this;
}

Point3D Ray3D::rayPosition(double lambda) const {
    return myOrigin + lambda*myDirection;
}

Ray3D Ray3D::bias(const Point3D &normal) const {
    return Ray3D(myOrigin + biasAmount*normal, myDirection);
}

Ray3D Ray3D::offset(Point3D amount) const {
    return Ray3D(myOrigin, myDirection + amount).normalized();
}

Ray3D Ray3D::normalized() const {
    return Ray3D(myOrigin, myDirection.normalized());
}

/* RAY DIFFERENTIAL */
RayDifferential::RayDifferential(Point3D origin, Point3D direction, double maxT, double time)
	: Ray3D(origin, direction, maxT, time)
{
	myHasDifferentials = false;
}

RayDifferential::RayDifferential(const Ray3D &ray)
	: Ray3D(ray)
{
	myHasDifferentials = false;
}

void RayDifferential::scaleDifferentials(double scale)
{
	myXOrigin = myOrigin + (myXOrigin - myOrigin) * scale;
	myYOrigin = myOrigin + (myYOrigin - myOrigin) * scale;
	myXDirection = myDirection + (myXDirection - myDirection) * scale;
	myYDirection = myDirection + (myYDirection - myDirection) * scale;
}