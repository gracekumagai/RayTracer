#include "ColourRGB.h"

ColourRGB::ColourRGB(double red, double green, double blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
}

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
