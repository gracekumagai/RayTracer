#ifndef Image_h
#define Image_h

#include <stdio.h>
#include "ColourRGB.h"

class Image {
public:
    unsigned char *rgbImageData;
    int sx;
    int sy;
    
    Image();    // Note: this constructor does not allocate space for the image data
    Image(int sx, int sy);
    Image(const Image &im);
    ~Image();
    
    Image& operator=(const Image &im);
    
    // Texture mapping function. Takes normalized texture coordinates (a,b) and returns the
    // texture colour at that point using bi-linear interpolation
    ColourRGB textureMap(double a, double b) const;
    
    static bool readPPMimage(const char *filename, Image *im);
    
    // Writes out a .ppm file from the image data contained in 'im'.
    // Note that Windows typically doesn't know how to open .ppm
    // images. Use Gimp or any other seious image processing
    // software to display .ppm images.
    // Also, note that because of Windows file format management,
    // you may have to modify this file to get image output on
    // Windows machines to work properly.
    //
    // Assumes a 24 bit per pixel image stored as unsigned chars
    //
    
    void setColourAtPixel(int x, int y, ColourRGB color);
    ColourRGB getColourAtPixel(int x, int y) const;
    void outputImage(const char *filename);
};

#endif
