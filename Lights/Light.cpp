#include "Light.h"

Light::Light(const ColourRGB &colour, const Point3D &location) :
	myColour(colour), myLocation(location) {}

ColourRGB Light::getColour() 
{
    return myColour;
}
