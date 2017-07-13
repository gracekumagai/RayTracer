#ifndef Ray3D_h
#define Ray3D_h

#include <stdio.h>
#include "../GeometricTypes/Point3D.h"

// forward declare the object type to avoid circular dependencies
class Object3D;

class Ray3D {
    const double biasAmount = 1e-6;
    
public:
    Point3D origin;
    Point3D direction;
    
    Ray3D(Point3D origin, Point3D direction);
    Ray3D(const Ray3D &ray);
    Ray3D& operator=(const Ray3D &ray);
    
    // Function for returning the position along the ray
    // for a given lambda.
    Point3D rayPosition(double lambda) const;
    
    Ray3D bias(const Point3D &normal) const;
    
    Ray3D offset(Point3D amount) const;
    
    Ray3D normalized() const;
};

#endif
