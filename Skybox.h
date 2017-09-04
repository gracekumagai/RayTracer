#ifndef Skybox_h
#define Skybox_h

#include "ObjectTypes/Image.h"
#include "GeometricTypes/Point3D.h"

class Skybox 
{
    Image myLeft, myRight, myBottom, myTop, myBack, myFront;
    
public:
    Skybox(const char *left, const char *right,
           const char *bottom, const char *top,
           const char *back, const char *front);
    
    ColourRGB colourInDirection(Point3D direction);
};

#endif /* Skybox_h */
