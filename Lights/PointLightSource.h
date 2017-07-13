#ifndef PointLightSource_h
#define PointLightSource_h

#include "Light.h"

class PointLightSource : public Light {
public:
    PointLightSource(const ColourRGB &colour, const Point3D &location);
    
    Point3D getLocation();
};

#endif
