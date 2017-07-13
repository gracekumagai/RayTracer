#include "Image.h"
#include <stdlib.h>
#include <string.h>

Image::Image() {
    sx = 0;
    sy = 0;
    rgbImageData = NULL;
}

Image::Image(int sx, int sy) {
    this->sx = sx;
    this->sy = sy;
    
    rgbImageData = (unsigned char *)calloc(sx*sy*3,sizeof(unsigned char));
    if (rgbImageData == NULL)
        fprintf(stderr,"Unable to allocate memory for new image\n");
}

Image::Image(const Image &im) {
    sx = im.sx;
    sy = im.sy;
    if (im.rgbImageData == NULL)
        rgbImageData = NULL;
    else {
        rgbImageData = (unsigned char *)calloc(im.sx*im.sy*3,sizeof(unsigned char));
        memcpy(rgbImageData, im.rgbImageData, im.sx*im.sy*3*sizeof(unsigned char));
    }
}

Image::~Image() {
    if (rgbImageData != NULL) free(rgbImageData);
}

Image& Image::operator=(const Image &im) {
    sx = im.sx;
    sy = im.sy;
    
    if (rgbImageData != NULL) free(rgbImageData);   // clear the old data
    
    if (im.rgbImageData == NULL)
        rgbImageData = NULL;
    else {
        rgbImageData = (unsigned char *)calloc(im.sx*im.sy*3,sizeof(unsigned char));
        memcpy(rgbImageData, im.rgbImageData, im.sx*im.sy*3*sizeof(unsigned char));
    }
    
    return *(this);
}

ColourRGB Image::textureMap(double a, double b) const {
    /*
     Function to determine the colour of a textured object at
     the normalized texture coordinates (a,b).
     
     a and b are texture coordinates in [0 1].
     img is a pointer to the image structure holding the texture for
     a given object.
     
     The colour is returned in R, G, B. Uses bi-linear interpolation
     to determine texture colour.
     */
    
    //////////////////////////////////////////////////
    // TO DO (Assignment 4 only):
    //
    //  Complete this function to return the colour
    // of the texture image at the specified texture
    // coordinates. Your code should use bi-linear
    // interpolation to obtain the texture colour.
    //////////////////////////////////////////////////
    
    // NOTE: Assuming that texture wraps around at the edges
    
    b = 1-b; // in immages space, b increases downwards
    
    int i = (int)(a*sx);
    if (i == sx) i -= 1;    // Was just at the edge of the last pixel
    int j = (int)(b*sy);
    if (j == sy) j -= 1;    // Was just at the edge of the last pixel
    
    double aPrime = a - (double)i/sx;
    double bPrime = b - (double)j/sy;
    
    return getColourAtPixel(i, j)*(1-aPrime)*(1-bPrime) +
        getColourAtPixel((i+1)%sx, j)*aPrime*(1-bPrime) +
        getColourAtPixel(i, (j+1)%sy)*(1-aPrime)*bPrime +
        getColourAtPixel((i+1)%sx, (j+1)%sy)*aPrime*bPrime;
}

bool Image::readPPMimage(const char *filename, Image *im) {
    // Reads an image from a .ppm file. A .ppm file is a very simple image representation
    // format with a text header followed by the binary RGB data at 24bits per pixel.
    // The header has the following form:
    //
    // P6
    // # One or more comment lines preceded by '#'
    // 340 200
    // 255
    //
    // The first line 'P6' is the .ppm format identifier, this is followed by one or more
    // lines with comments, typically used to inidicate which program generated the
    // .ppm file.
    // After the comments, a line with two integer values specifies the image resolution
    // as number of pixels in x and number of pixels in y.
    // The final line of the header stores the maximum value for pixels in the image,
    // usually 255.
    // After this last header line, binary data stores the RGB values for each pixel
    // in row-major order. Each pixel requires 3 bytes ordered R, G, and B.
    //
    // NOTE: Windows file handling is rather crotchetty. You may have to change the
    //       way this file is accessed if the images are being corrupted on read
    //       on Windows.
    //
    // readPPMdata converts the image colour information to floating point. This is so that
    // the texture mapping function doesn't have to do the conversion every time
    // it is asked to return the colour at a specific location.
    //
    
    FILE *f;
    char line[1024];
    int sizx,sizy;
    unsigned char *rgb;
    
    if (im!=NULL)
    {
        im->rgbImageData = NULL;
        f = fopen(filename,"rb+");
        if (f == NULL)
        {
            fprintf(stderr,"Unable to open file %s for reading, please check name and path\n", filename);
            return false;
        }
        if (fgets(&line[0],1000,f))
            ;
        if (strcmp(&line[0],"P6\n")!=0)
        {
            fprintf(stderr,"Wrong file format, not a .ppm file or header end-of-line characters missing\n");
            fclose(f);
            return false;
        }
        fprintf(stderr,"%s\n",line);
        // Skip over comments
        if (fgets(&line[0],511,f))
            ;
        while (line[0]=='#')
        {
            fprintf(stderr,"%s",line);
            if (fgets(&line[0],511,f))
                ;
        }
        sscanf(&line[0],"%d %d\n",&sizx,&sizy);           // Read file size
        fprintf(stderr,"nx=%d, ny=%d\n\n",sizx,sizy);
        im->sx=sizx;
        im->sy=sizy;
        
        if (fgets(&line[0],9,f))    // Read the remaining header line
            ;
        fprintf(stderr,"%s\n",line);
        rgb=(unsigned char *)calloc(sizx*sizy*3,sizeof(unsigned char));
        if (rgb==NULL)
        {
            fprintf(stderr,"Out of memory allocating space for image\n");
            fclose(f);
            return false;
        }
        
        size_t sizeRead = fread(rgb,sizx*sizy*3*sizeof(unsigned char),1,f);
        fclose(f);
        
        // Conversion to floating point
        im->rgbImageData=rgb;
        return true;
    }
    
    fprintf(stderr,"Unable to allocate memory for image structure\n");
    return false;
}

void Image::setColourAtPixel(int x, int y, ColourRGB colour) {
    if (x >= sx || y >= sy) {
        printf("Error: pixel is out of bounds of the image\n");
    }
    
    if (colour.red < 0.0 || colour.red > 1.0 ||
        colour.green < 0.0 || colour.green > 1.0 ||
        colour.blue < 0.0 || colour.blue > 1.0) {
        printf("Error: setting pixel to an invalid colour\n");
    }
    
    unsigned char R = (unsigned char)(int)(255 * colour.red);
    unsigned char G = (unsigned char)(int)(255 * colour.green);
    unsigned char B = (unsigned char)(int)(255 * colour.blue);
    
    *((unsigned char *)rgbImageData + 3*(y*sx + x) + 0) = R;
    *((unsigned char *)rgbImageData + 3*(y*sx + x) + 1) = G;
    *((unsigned char *)rgbImageData + 3*(y*sx + x) + 2) = B;
}

ColourRGB Image::getColourAtPixel(int x, int y) const {
    if (x >= sx || y >= sy) {
        printf("Error: pixel is out of bounds of the image\n");
        return ColourRGB(-1, -1, -1);   // return invalid colour
    }
    
    unsigned char R = *((unsigned char *)rgbImageData + 3*(y*sx + x) + 0);
    unsigned char G = *((unsigned char *)rgbImageData + 3*(y*sx + x) + 1);
    unsigned char B = *((unsigned char *)rgbImageData + 3*(y*sx + x) + 2);
    
    return ColourRGB(R/255.0, G/255.0, B/255.0);
}

void Image::outputImage(const char *filename) {
    FILE *f;
    
    if (rgbImageData != NULL) {
        f=fopen(filename,"wb+");
        if (f==NULL)
        {
            fprintf(stderr,"Unable to open file %s for output! No image written\n",filename);
            return;
        }
        fprintf(f,"P6\n");
        fprintf(f,"# Output from RayTracer.c\n");
        fprintf(f,"%d %d\n", sx, sy);
        fprintf(f,"255\n");
        fwrite((unsigned char *)rgbImageData, sx*sy*3*sizeof(unsigned char),1,f);
        fclose(f);
        return;
    }
    fprintf(stderr,"imageOutput(): Specified image is empty. Nothing output\n");
}

