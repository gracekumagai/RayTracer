#include "Skybox.h"
#include <math.h>

Skybox::Skybox(const char *left, const char *right,
               const char *bottom, const char *top,
               const char *back, const char *front) {
    Image::readPPMimage(left, &(this->left));
    Image::readPPMimage(right, &(this->right));
    Image::readPPMimage(bottom, &(this->bottom));
    Image::readPPMimage(top, &(this->top));
    Image::readPPMimage(back, &(this->back));
    Image::readPPMimage(front, &(this->front));
}

ColourRGB Skybox::colourInDirection(Point3D direction) {
    bool isXPos = direction.x > 0;
    bool isYPos = direction.y > 0;
    bool isZPos = direction.z > 0;
    double absX = fabs(direction.x);
    double absY = fabs(direction.y);
    double absZ = fabs(direction.z);
    
    double maxAxis, uc, vc;
    Image *image;
    
    // Positive x
    if (isXPos && absX >= absY && absX >= absZ) {
        // u (0 to 1) goes from +z to -z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = -direction.z;
        vc = direction.y;
        image = &(this->front);
    }
    // NEGATIVE X
    else if (!isXPos && absX >= absY && absX >= absZ) {
        // u (0 to 1) goes from -z to +z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = direction.z;
        vc = direction.y;
        image = &(this->back);
    }
    // POSITIVE Y
    else if (isYPos && absY >= absX && absY >= absZ) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from +z to -z
        maxAxis = absY;
        uc = direction.x;
        vc = -direction.z;
        image = &(this->top);
    }
    // NEGATIVE Y
    else if (!isYPos && absY >= absX && absY >= absZ) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -z to +z
        maxAxis = absY;
        uc = direction.x;
        vc = direction.z;
        image = &(this->bottom);
    }
    // POSITIVE Z
    else if (isZPos && absZ >= absX && absZ >= absY) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = direction.x;
        vc = direction.y;
        image = &(this->left);
    }
    // NEGATIVE Z
    else {
        // u (0 to 1) goes from +x to -x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = -direction.x;
        vc = direction.y;
        image = &(this->right);
    }
    
    // Convert range from -1 to 1 to 0 to 1
    double u = 0.5 * (uc / maxAxis + 1.0);
    double v = 0.5 * (vc / maxAxis + 1.0);
    
    return image->textureMap(u, v);
}
