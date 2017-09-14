#ifndef Image_h
#define Image_h

#include <stdio.h>
#include "ColourRGB.h"

class Image 
{
public:
    unsigned char *myRGBImageData;
    int myX;
    int myY;
    
    Image();
    Image(int sx, int sy);
    Image(const Image &im);
    ~Image();
    
    Image& operator=(const Image &im);
    
    // Texture mapping function. Takes normalized texture coordinates (a,b) and returns the
    // texture colour at that point using bi-linear interpolation
    ColourRGB textureMap(double a, double b) const;
    
    static bool readPPMimage(const char *filename, Image *im);
    
    // Writes out a .ppm file from the image data contained in 'im'.
    void setColourAtPixel(int x, int y, ColourRGB color);
	void setColourAtPixel(int index, ColourRGB color);

    ColourRGB getColourAtPixel(int x, int y) const;
	ColourRGB getColourAtPixel(int index) const;
    
	void outputImage(const char *filename);
};
#endif
