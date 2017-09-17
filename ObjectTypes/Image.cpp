#include "Image.h"
#include <stdlib.h>
#include <string.h>

Image::Image() 
{
    myX = 0;
    myY = 0;
    myRGBImageData = NULL;
}

Image::Image(int sx, int sy)
{
	this->myX = sx;
	this->myY = sy;

	myRGBImageData = (unsigned char *)calloc(sx*sy * 3, sizeof(unsigned char));
	if (myRGBImageData == NULL)
	{
		fprintf(stderr, "Unable to allocate memory for new image\n");
	}
}

Image::Image(const Image &im) 
{
    myX = im.myX;
    myY = im.myY;
	if (im.myRGBImageData == NULL)
	{
		myRGBImageData = NULL;
	}
	else 
	{
        myRGBImageData = (unsigned char *)calloc(im.myX*im.myY*3,sizeof(unsigned char));
        memcpy(myRGBImageData, im.myRGBImageData, im.myX*im.myY*3*sizeof(unsigned char));
    }
}

Image::~Image() 
{
    if (myRGBImageData != NULL) free(myRGBImageData);
}

Image& Image::operator=(const Image &im) 
{
    myX = im.myX;
    myY = im.myY;
    
	if (myRGBImageData != NULL)
	{
		free(myRGBImageData);   // clear the old data
	}

	if (im.myRGBImageData == NULL)
	{
		myRGBImageData = NULL;
	}
	else 
	{
        myRGBImageData = (unsigned char *)calloc(im.myX*im.myY*3,sizeof(unsigned char));
        memcpy(myRGBImageData, im.myRGBImageData, im.myX*im.myY*3*sizeof(unsigned char));
    }
    
    return *(this);
}

ColourRGB Image::textureMap(double a, double b) const 
{
	/*
	Function to determine the colour of a textured object at
	the normalized texture coordinates (a,b).

	Assuming that texture wraps around at the edges
	*/
    
    b = 1 - b;				// in images space, b increases downwards
    
    int i = (int)(a*myX);
    if (i == myX) i -= 1;	// Was just at the edge of the last pixel
    int j = (int)(b*myY);
    if (j == myY) j -= 1;	// Was just at the edge of the last pixel
    
    double aPrime = a - (double)i/myX;
    double bPrime = b - (double)j/myY;
    
    return getColourAtPixel(i, j)*(1-aPrime)*(1-bPrime) +
        getColourAtPixel((i+1)%myX, j)*aPrime*(1-bPrime) +
        getColourAtPixel(i, (j+1)%myY)*(1-aPrime)*bPrime +
        getColourAtPixel((i+1)%myX, (j+1)%myY)*aPrime*bPrime;
}

bool Image::readPPMimage(const char *filename, Image *im) 
{
    // Reads an image from a .ppm file. 
    
    FILE *f;
    char line[1024];
    int sizx,sizy;
    unsigned char *rgb;
    
    if (im!=NULL)
    {
        im->myRGBImageData = NULL;
        f = fopen(filename,"rb+");
        if (f == NULL)
        {
            fprintf(stderr,"Unable to open file %s for reading, please check name and path\n", filename);
            return false;
        }
        if (fgets(&line[0],1000,f))
            ;
        if (strcmp(&line[0],"P6\n")!=0 && strcmp(&line[0], "P6\r\n")!=0)
        {
            fprintf(stderr,"Wrong file format, not a .ppm file or header end-of-line characters missing\n");
            fclose(f);
            return false;
        }
        fprintf(stderr,"%s\n",line);
        if (fgets(&line[0],511,f))						// Skip over comments
            ;
        while (line[0]=='#')
        {
            fprintf(stderr,"%s",line);
            if (fgets(&line[0],511,f))
                ;
        }
        sscanf(&line[0],"%d %d\n",&sizx,&sizy);			// Read file size
        fprintf(stderr,"nx=%d, ny=%d\n\n",sizx,sizy);
        im->myX=sizx;
        im->myY=sizy;
        
        if (fgets(&line[0],9,f))						// Read the remaining header line
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
        im->myRGBImageData=rgb;
        return true;
    }
    
    fprintf(stderr,"Unable to allocate memory for image structure\n");
    return false;
}

void Image::setColourAtPixel(int x, int y, ColourRGB colour) 
{
    if (x >= myX || y >= myY) {
        printf("Error: pixel is out of bounds of the image\n");
    }
    
    if (colour.myRed < 0.0 || colour.myRed > 1.0 ||
        colour.myGreen < 0.0 || colour.myGreen > 1.0 ||
        colour.myBlue < 0.0 || colour.myBlue > 1.0) {
        printf("Error: setting pixel to an invalid colour\n");
    }
    
    unsigned char R = (unsigned char)(int)(255 * colour.myRed);
    unsigned char G = (unsigned char)(int)(255 * colour.myGreen);
    unsigned char B = (unsigned char)(int)(255 * colour.myBlue);
    
    *((unsigned char *)myRGBImageData + 3*(y*myX + x) + 0) = R;
    *((unsigned char *)myRGBImageData + 3*(y*myX + x) + 1) = G;
    *((unsigned char *)myRGBImageData + 3*(y*myX + x) + 2) = B;
}

void Image::setColourAtPixel(int index, ColourRGB colour)
{
	if (index > myX * myY) {
		printf("Error: pixel is out of bounds of the image\n");
	}

	if (colour.myRed < 0.0 || colour.myRed > 1.0 ||
		colour.myGreen < 0.0 || colour.myGreen > 1.0 ||
		colour.myBlue < 0.0 || colour.myBlue > 1.0) {
		printf("Error: setting pixel to an invalid colour\n");
	}

	unsigned char R = (unsigned char)(int)(255 * (colour.myRed));
	unsigned char G = (unsigned char)(int)(255 * (colour.myGreen));
	unsigned char B = (unsigned char)(int)(255 * (colour.myBlue));

	*((unsigned char *)myRGBImageData + 3 * index + 0) = R;
	*((unsigned char *)myRGBImageData + 3 * index + 1) = G;
	*((unsigned char *)myRGBImageData + 3 * index + 2) = B;
}

void Image::addColourAtPixel(int x, int y, ColourRGB colour)
{
	if (x >= myX || y >= myY) {
		printf("Error: pixel is out of bounds of the image\n");
	}

	if (colour.myRed < 0.0 || colour.myRed > 1.0 ||
		colour.myGreen < 0.0 || colour.myGreen > 1.0 ||
		colour.myBlue < 0.0 || colour.myBlue > 1.0) {
		printf("Error: setting pixel to an invalid colour\n");
	}

	unsigned char R = (unsigned char)(int)(255 * colour.myRed);
	unsigned char G = (unsigned char)(int)(255 * colour.myGreen);
	unsigned char B = (unsigned char)(int)(255 * colour.myBlue);

	*((unsigned char *)myRGBImageData + 3 * (y*myX + x) + 0) += R;
	*((unsigned char *)myRGBImageData + 3 * (y*myX + x) + 1) += G;
	*((unsigned char *)myRGBImageData + 3 * (y*myX + x) + 2) += B;
}

ColourRGB Image::getColourAtPixel(int x, int y) const 
{
    if (x >= myX || y >= myY) 
	{
        printf("Error: pixel is out of bounds of the image\n");
        return ColourRGB(-1, -1, -1);   // return invalid colour
    }
    
    unsigned char R = *((unsigned char *)myRGBImageData + 3*(y*myX + x) + 0);
    unsigned char G = *((unsigned char *)myRGBImageData + 3*(y*myX + x) + 1);
    unsigned char B = *((unsigned char *)myRGBImageData + 3*(y*myX + x) + 2);
    
    return ColourRGB(R/255.0, G/255.0, B/255.0);
}

ColourRGB Image::getColourAtPixel(int index) const 
{
	if (index >= myX * myY) 
	{
		printf("Error: pixel is out of bounds of the image\n");
		return ColourRGB(-1, -1, -1);   // return invalid colour
	}

	unsigned char R = *((unsigned char *)myRGBImageData + 3 * index + 0);
	unsigned char G = *((unsigned char *)myRGBImageData + 3 * index + 1);
	unsigned char B = *((unsigned char *)myRGBImageData + 3 * index + 2);

	return ColourRGB(R / 255.0, G / 255.0, B / 255.0);
}

void Image::outputImage(const char *filename) 
{
    FILE *f;
    
    if (myRGBImageData != NULL) 
	{
        f=fopen(filename,"wb+");
        if (f==NULL)
        {
            fprintf(stderr,"Unable to open file %s for output! No image written\n",filename);
            return;
        }
        fprintf(f,"P6\n");
        fprintf(f,"# Output from RayTracer.c\n");
        fprintf(f,"%d %d\n", myX, myY);
        fprintf(f,"255\n");
        fwrite((unsigned char *)myRGBImageData, myX*myY*3*sizeof(unsigned char),1,f);
        fclose(f);
        return;
    }
    fprintf(stderr,"imageOutput(): Specified image is empty. Nothing output\n");
}

void Image::createBlackImage()
{
	for (int i = 0; i < myX; i++)
	{
		for (int j = 0; j < myY; j++)
		{
			setColourAtPixel(i, j, ColourRGB(0, 0, 0));
		}
	}
}
