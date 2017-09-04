#include "ColourRGB.h"

ColourRGB::ColourRGB(const double red = 0, const double green = 0, const double blue = 0) : 
	red(red), green(green), blue(blue) { }

ColourRGB::ColourRGB(const ColourRGB &colour) {
    red = colour.red;
    green = colour.green;
    blue = colour.blue;
}

ColourRGB& ColourRGB::operator=(const ColourRGB &colour) {
    red = colour.red;
    green = colour.green;
    blue = colour.blue;
    
    return *this;
}

ColourRGB ColourRGB::operator+(const ColourRGB &colour) {
    return ColourRGB(red + colour.red, green + colour.green, blue + colour.blue);
}

ColourRGB& ColourRGB::operator+=(const ColourRGB &colour) {
    red = red + colour.red;
    green = green + colour.green;
    blue = blue + colour.blue;
    
    return *this;
}

ColourRGB ColourRGB::operator*(double scale) const {
    return ColourRGB(red*scale, green*scale, blue*scale);
}

ColourRGB ColourRGB::filter(const ColourRGB &filterColour) {
    return ColourRGB(red * filterColour.red,
                     green * filterColour.green,
                     blue * filterColour.blue);
}

bool ColourRGB::outOfBounds() {
    return red > 1.0 || green > 1.0 || blue > 1.0;
}

void ColourRGB::normalize() {
    if (red > 1.0) red = 1.0;
    if (green > 1.0) green = 1.0;
    if (blue > 1.0) blue = 1.0;
}

ColourRGB ColourRGB::lerp(ColourRGB a, ColourRGB b, double t)
{
	double red = a.red + t*(b.red - a.red);
	double green = a.green + t*(b.green - a.green);
	double blue = a.blue + t*(b.blue - a.blue);

	return ColourRGB(red, green, blue);
}

Point3D ColourRGB::convertToPoint3D(bool isVec)
{
	return Point3D(red, green, blue, isVec);
}

ColourRGB ColourRGB::createHue(double h)
{
	h *= 6.0;
	int hi = static_cast<int>(h);
	double hf = h - hi;

	switch (hi % 6)
	{
	case 0:
		return ColourRGB(1.0, hf, 0.0);
	case 1:
		return ColourRGB(1.0 - hf, 1.0, 0.0);
	case 2:
		return ColourRGB(0.0, 1.0, hf);
	case 3:
		return ColourRGB(0.0, 1.0 - hf, 1.0);
	case 4:
		return ColourRGB(hf, 0.0, 1.0);
	case 5:
		return ColourRGB(1.0, 0.0, 1.0 - hf);
	}
	return ColourRGB(0.0, 0.0, 0.0);
}