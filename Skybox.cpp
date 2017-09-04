#include "Skybox.h"
#include <math.h>

Skybox::Skybox(const char *left, const char *right,
               const char *bottom, const char *top,
               const char *back, const char *front) 
{
    Image::readPPMimage(left, &(this->myLeft));
    Image::readPPMimage(right, &(this->myRight));
    Image::readPPMimage(bottom, &(this->myBottom));
    Image::readPPMimage(top, &(this->myTop));
    Image::readPPMimage(back, &(this->myBack));
    Image::readPPMimage(front, &(this->myFront));
}

ColourRGB Skybox::colourInDirection(Point3D direction) 
{
    bool isXPos = direction.x > 0;
    bool isYPos = direction.y > 0;
    bool isZPos = direction.z > 0;
    double absX = fabs(direction.x);
    double absY = fabs(direction.y);
    double absZ = fabs(direction.z);
    
    double maxAxis, uc, vc;
    Image *image;
    
    // POSITIVE x
    if (isXPos && absX >= absY && absX >= absZ) 
	{
        // u (0 to 1) goes from +z to -z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = -direction.z;
        vc = direction.y;
        image = &(this->myFront);
    }
    // NEGATIVE X
    else if (!isXPos && absX >= absY && absX >= absZ) 
	{
        // u (0 to 1) goes from -z to +z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = direction.z;
        vc = direction.y;
        image = &(this->myBack);
    }
    // POSITIVE Y
    else if (isYPos && absY >= absX && absY >= absZ) 
	{
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from +z to -z
        maxAxis = absY;
        uc = direction.x;
        vc = -direction.z;
        image = &(this->myTop);
    }
    // NEGATIVE Y
    else if (!isYPos && absY >= absX && absY >= absZ) 
	{
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -z to +z
        maxAxis = absY;
        uc = direction.x;
        vc = direction.z;
        image = &(this->myBottom);
    }
    // POSITIVE Z
    else if (isZPos && absZ >= absX && absZ >= absY) 
	{
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = direction.x;
        vc = direction.y;
        image = &(this->myLeft);
    }
    // NEGATIVE Z
    else {
        // u (0 to 1) goes from +x to -x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = -direction.x;
        vc = direction.y;
        image = &(this->myRight);
    }
    
    // Convert range from -1 to 1 to 0 to 1
    double u = 0.5 * (uc / maxAxis + 1.0);
    double v = 0.5 * (vc / maxAxis + 1.0);
    
    return image->textureMap(u, v);
}
