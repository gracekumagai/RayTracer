#ifndef Plane_h
#define Plane_h

#include "../Object3D.h"
#include <stdio.h>

class Plane : public Object3D {
    //Point on plane defined CCQ
    const Point3D p1 = Point3D(0.5, 0.5, 0, false);
    const Point3D p2 = Point3D(-0.5, 0.5, 0, false);
    const Point3D p3 = Point3D(-0.5, -0.5, 0, false);
    const Point3D p4 = Point3D(0.5, -0.5, 0, false);
    const Point3D c = Point3D(0.0, 0.0, 0.0, false);
    const Point3D normal = Point3D(0, 0, 1, true);
    
    ColourRGB colourAtLocalPoint(const Point3D &p) const;
    
public:
    Plane(const Material &material, const ColourRGB &colour);
    
    Intersection intersect(const Ray3D &ray);
    
    bool doesIntersect(const Ray3D &ray);
};

#endif
