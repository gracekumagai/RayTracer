#include "Point3D.h"
#include "../ObjectTypes/Object3D.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Point3D::Point3D() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
    w = 1.0;    // default to point, even though can be vec if this is 0.0
}

Point3D::Point3D(double x, double y, double z, bool isVec) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = isVec ? 0.0 : 1.0;
}

Point3D::Point3D(objl::Vector3 &v, bool isVec) {
	this->x = v.X;
	this->y = v.Y;
	this->z = v.Z;
	this->w = isVec ? 0.0 : 1.0;
}

Point3D::Point3D(Point3D &p1, Point3D &p2, Point3D &p3, bool isVec) {
	//Average constructor
	this->x = (p1.x + p2.x + p3.x)/3;
	this->y = (p1.y + p2.y + p3.y)/3;
	this->z = (p1.z + p2.z + p3.z)/3;
    this->w = isVec ? 0.0 : 1.0;
}

// Should only be called with vectors (w=0), since points don't have magnitudes
double Point3D::magnitude() const {
    return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

Point3D Point3D::normalized() const {
    return 1.0/(this->magnitude()) * (*this);
}

// Whenever this is called in order to compute the actual dot product, the contribution
// from the w components should be 0, since the dot product only makes sense with vectors.
// The reason that we still add it into the dot product over here is so that we can use this
// function in matrix multiplication.
double Point3D::dot(const Point3D &other) const {
    return this->x*other.x + this->y*other.y + this->z*other.z + this->w*other.w;
}

// Assume that this is called when either both inputs
// are points (w=1) or both inputs are vectors (w=0)
Point3D Point3D::crossUnit(const Point3D &other) const {
    return Point3D(this->y*other.z - this->z*other.y,
                other.x*this->z - other.z*this->x,
                this->x*other.y - this->y*other.x,
                this->w == 0.0 ? 0.0 : 1.0)
    .normalized();
}

Point3D Point3D::operator+(const Point3D &other) const {
    return Point3D(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w); // only really makes sense for vectors (w=0)
}

Point3D Point3D::operator-(const Point3D &other) const {
    return Point3D(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
}

Point3D Point3D::operator*(double scale) const {
    return Point3D(this->x * scale, this->y * scale, this->z * scale, this->w); // uniform scale transform does not effect w
}

bool Point3D::operator==(const Point3D &other) const {
    return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w;
}

bool Point3D::operator!=(const Point3D &other) const {
    return !(*(this) == other);
}

Point3D Point3D::linearInterpolate(const Point3D &end, double progress) const {
    Point3D difference = end - (*this);
    return (*this) + difference*progress;
}

Point3D Point3D::randomlyPerturb(const Point3D &normal, double degree) const {
    // Create orthonormal basis at intersection point
    Point3D u, v;
    if (fabs(Point3D(1, 0, 0, true).dot(*this)) > 1.0 - 1e-6)
        u = (*this).crossUnit(Point3D(0, 1, 0, true));
    else
        u = (*this).crossUnit(Point3D(1, 0, 0, true));
    v = (*this).crossUnit(u);
    
    // Choose uniformly sampled random direction to send the ray in
    double theta = (M_PI * drand48() - M_PI/2.0) * degree;  // -pi*degree/2 to pi*degree/2
    double phi = 2 * M_PI * (drand48());                    // 0 to 2pi
    double x = sin(theta)*cos(phi);
    double y = sin(theta)*sin(phi);
    double z = cos(theta);
    
    // Convert sample to world coord using the orthonormal basis
    Point3D perturbed = ((x * u) + (y * v) + (z * (*this))).normalized();
    
    // Check if below the surface, and if so, invert the u and v components
    if (perturbed.dot(normal) < 0) {
        perturbed = ((-x * u) + (-y * v) + (z * (*this))).normalized();
    }
    
    return perturbed;
}

void Point3D::printPoint3D() const {
    printf("(%.2f, %.2f, %.2f, %.2f)\n", x, y, z, w);
}

Point3D operator*(double scale, const Point3D &p) {
    return p*scale;
}

Point3D::Point3D(double x, double y, double z, double w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Point3D Point3D::homogeonized() const {
    if (this->w == 0.0) {
        return (*this);
    }
    else {
        return Point3D(this->x/this->w, this->y/this->w, this->z/this->w, 1.0);
    }
}
double Point3D::average() {
	return ((this->x + this->y + this->z)/3);
}


