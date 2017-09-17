#ifndef Disk_h
#define Disk_h

#include "../Object3D.h"
#include <stdio.h>

#define PI 3.14159265354

class Disk : public Object3D {
	double myRadius = 2.0;
	double myInnerRadius = 1.0;
	double myPhiMax = 2 * PI;
	Point3D myCenter = Point3D(0.0, 0.0, 0.0, false);
    
    ColourRGB colourAtLocalPoint(const Point3D &p) const;
    
public:
    Disk(const Material &material, const ColourRGB &colour);
	Disk(double height, double r, double ri, double t, const Material &material, const ColourRGB &colour);

	double area() const;

    Intersection intersect(const Ray3D &ray);
    
    bool doesIntersect(const Ray3D &ray);
};

#endif
