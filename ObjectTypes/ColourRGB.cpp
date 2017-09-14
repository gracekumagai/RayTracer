#include "ColourRGB.h"

ColourRGB::ColourRGB(const double red = 0, const double green = 0, const double blue = 0) : 
	myRed(red), myGreen(green), myBlue(blue) { }

ColourRGB::ColourRGB(const ColourRGB &colour) 
{
    myRed = colour.myRed;
    myGreen = colour.myGreen;
    myBlue = colour.myBlue;
}

ColourRGB& ColourRGB::operator=(const ColourRGB &colour) 
{
    myRed = colour.myRed;
    myGreen = colour.myGreen;
    myBlue = colour.myBlue;
    
    return *this;
}

ColourRGB ColourRGB::operator+(const ColourRGB &colour) 
{
    return ColourRGB(myRed + colour.myRed, myGreen + colour.myGreen, myBlue + colour.myBlue);
}

ColourRGB& ColourRGB::operator+=(const ColourRGB &colour) 
{
    myRed = myRed + colour.myRed;
    myGreen = myGreen + colour.myGreen;
    myBlue = myBlue + colour.myBlue;
    
    return *this;
}

ColourRGB ColourRGB::operator*(double scale) const 
{
    return ColourRGB(myRed*scale, myGreen*scale, myBlue*scale);
}

ColourRGB ColourRGB::filter(const ColourRGB &filterColour) 
{
    return ColourRGB(myRed * filterColour.myRed,
                     myGreen * filterColour.myGreen,
                     myBlue * filterColour.myBlue);
}

bool ColourRGB::outOfBounds() 
{
    return myRed > 1.0 || myGreen > 1.0 || myBlue > 1.0;
}

void ColourRGB::normalize() 
{
    if (myRed > 1.0) myRed = 1.0;
    if (myGreen > 1.0) myGreen = 1.0;
    if (myBlue > 1.0) myBlue = 1.0;
}

ColourRGB ColourRGB::lerp(ColourRGB a, ColourRGB b, double t)
{
	double red = a.myRed + t*(b.myRed - a.myRed);
	double green = a.myGreen + t*(b.myGreen - a.myGreen);
	double blue = a.myBlue + t*(b.myBlue - a.myBlue);

	return ColourRGB(red, green, blue);
}

Point3D ColourRGB::convertToPoint3D(bool isVec)
{
	return Point3D(myRed, myGreen, myBlue, isVec);
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
