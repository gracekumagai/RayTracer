#ifndef Torus_h
#define Torus_h

#include "../Object3D.h"
#include <stdio.h>

#define PI 3.14159265354

using namespace std;

class Torus : public Object3D 
{
	// http://users.wowway.com/~phkahler/torus.pdf

	Point3D myCenter = Point3D(0.0, 0.0, 0.0, false);
	Point3D myAxis = Point3D(0.0, 1.0, 0.0, true);
	double myMajorR = 3.0;
	double myMinorR = 1.0;

    ColourRGB colourAtLocalPoint(const Point3D &p) const;
    
public:
	Torus(const Material &material, const ColourRGB &colour);
	Torus(Point3D center, Point3D axis, double major, double minor, const Material &material, const ColourRGB &colour);

	double area() const;

    Intersection intersect(const Ray3D &ray);
    
    bool doesIntersect(const Ray3D &ray);
};

#endif
