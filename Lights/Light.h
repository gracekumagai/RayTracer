#ifndef Light_h
#define Light_h

#include "../GeometricTypes/Point3D.h"
#include "../ObjectTypes/ColourRGB.h"

class Light 
{
protected:
    ColourRGB myColour;
    Point3D myLocation;
    
public:
    Light(const ColourRGB &colour, const Point3D &location);
    virtual ~Light(){}
    
    virtual Point3D getLocation() = 0;
    ColourRGB getColour();
};

#endif /* Light_h */
