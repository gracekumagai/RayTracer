#ifndef ColourRGB_h
#define ColourRGB_h

#include <stdio.h>
#include "../GeometricTypes/Point3D.h"

class ColourRGB {
public:
    double red;
    double green;
    double blue;
    
    ColourRGB(double red, double green, double blue);
    ColourRGB(){}
    ColourRGB(const ColourRGB &colour);
    
    ColourRGB& operator=(const ColourRGB &colour);
    ColourRGB operator+(const ColourRGB &colour);
    ColourRGB& operator+=(const ColourRGB &colour);
    ColourRGB operator*(double scale) const;
    
    ColourRGB filter(const ColourRGB &filterColour);
	static ColourRGB lerp(ColourRGB a, ColourRGB b, double t);

	Point3D convertToPoint3D(bool isVec);
	static ColourRGB createHue(double h);

    bool outOfBounds();
    void normalize();
};

#endif
