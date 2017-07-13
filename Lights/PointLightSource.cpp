#include <stdlib.h>
#include "PointLightSource.h"

PointLightSource::PointLightSource(const ColourRGB &colour, const Point3D &location) :
Light(colour, location) {}

Point3D PointLightSource::getLocation() {
    return Light::location;
}
