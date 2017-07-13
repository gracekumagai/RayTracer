#ifndef Light_h
#define Light_h

#include "../GeometricTypes/Point3D.h"
#include "../ObjectTypes/ColourRGB.h"

class Light {
protected:
    ColourRGB colour;
    Point3D location;
    
public:
    Light(const ColourRGB &colour, const Point3D &location);
    virtual ~Light(){}
    
    virtual Point3D getLocation() = 0;
    ColourRGB getColour();
};

#endif /* Light_h */
