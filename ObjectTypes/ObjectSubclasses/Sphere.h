#ifndef Sphere_h
#define Sphere_h

#include "../Object3D.h"
#include <stdio.h>

class Sphere : public Object3D {
    const Point3D centre = Point3D(0, 0, 0, false);
    const double radius = 1.0;
    
    ColourRGB colourAtLocalPoint(const Point3D &p) const;
    
public:
    Sphere(const Material &material, const ColourRGB &colour);
    
    Intersection intersect(const Ray3D &ray);
    
    bool doesIntersect(const Ray3D &ray);
};

#endif
