#ifndef ColourRGB_h
#define ColourRGB_h

#include <stdio.h>

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
    
    bool outOfBounds();
    void normalize();
};

#endif
