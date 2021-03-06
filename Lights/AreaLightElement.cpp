#include "AreaLightElement.h"
#include "../ObjectTypes/ObjectSubclasses/PlaneLightSource.h"
#include "../Utilities/Math.h"
#include <stdlib.h>

AreaLightElement::AreaLightElement(const ColourRGB &colour, const Point3D &location,
                                   const Point3D &xBasis, const Point3D &yBasis,
                                   double xMaxOffset, double yMaxOffset) :
	Light(colour, location) 
{
    this->myXBasis = xBasis.normalized();
    this->myYBasis = yBasis.normalized();
    this->myXMaxOffset = xMaxOffset;
    this->myYMaxOffset = yMaxOffset;
}

// Randomly offset the location to introduce noise.
// When all elements of an area light source are doing this,
// the resulting noise gets rid of bands in the shadow
Point3D AreaLightElement::getLocation() 
{
    return Light::myLocation + (drand48()*myXMaxOffset*myXBasis) + (drand48()*myYMaxOffset*myYBasis);
}

void AreaLightElement::addAreaLight(double width, double height, const Point3D &normal,
                                    const Point3D &xUnit, const Point3D &centre,
                                    int numHori, int numVert, const ColourRGB &colour,
                                    list<Light*> &lights, list<Object3D*> *objects) 
{   
    Point3D zBasis = normal.normalized();
    Point3D xBasis = xUnit.normalized();
    Point3D yBasis = zBasis.crossUnit(xBasis);
    
    // Place the actual point light sources
    Point3D botLeftCorner = centre - (width/2)*xBasis - (height/2)*yBasis;
    double deltaX = width / numHori;
    double deltaY = height / numVert;
    for (int i = 0; i < numHori; i++) 
	{
        for (int j = 0; j < numVert; j++) 
		{
            Point3D lightLocation = botLeftCorner + ((i*deltaX)*xBasis) + ((j*deltaY)*yBasis);
            AreaLightElement *light = new AreaLightElement(colour * (1.0/(numHori*numVert)), lightLocation,
                                                           xBasis, yBasis, deltaX, deltaY);
            lights.push_front(light);
        }
    }
    
    // Add the actual PlaneLightSource object so that we can see it in the scene,
    // if the user specified a value for 'objects' (default is NULL)
    if (objects != NULL) 
	{
        PlaneLightSource *lightPlane = new PlaneLightSource(colour, centre, xBasis, yBasis, zBasis, width, height);
        objects->push_front(lightPlane);
    }
}
