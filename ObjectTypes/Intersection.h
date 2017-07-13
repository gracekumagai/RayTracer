#ifndef Intersection_h
#define Intersection_h

#include "../GeometricTypes/Point3D.h"
#include "Material.h"
#include "ColourRGB.h"

class Intersection {
public:
    
    Point3D point;
    Point3D normal;
    Material material;
    ColourRGB colour;
    double lambda;
    
    bool none;
    bool insideObject;
    bool isLight;
    
    // Optimization if we know object can't reflect onto itself
    // at the specified intersection
    bool canSelfReflect;
    
    Object3D *obj;
    
    // No member constructor because this object is meant to be instantiated and
    // filled as we go along and compute different parts of the intersection.
    Intersection(){}
};

#endif
