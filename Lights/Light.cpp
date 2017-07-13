#include "Light.h"

Light::Light(const ColourRGB &colour, const Point3D &location) :
colour(colour), location(location) {}

ColourRGB Light::getColour() {
    return colour;
}
