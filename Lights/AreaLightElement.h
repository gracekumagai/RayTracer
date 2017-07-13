#ifndef AreaLightElement_h
#define AreaLightElement_h

#include "Light.h"
#include <list>

using namespace std;

class AreaLightElement : public Light {
    Point3D xBasis;
    Point3D yBasis;
    double xMaxOffset;
    double yMaxOffset;
    
public:
    AreaLightElement(const ColourRGB &colour, const Point3D &location,
                     const Point3D &xBasis, const Point3D &yBasis,
                     double xMaxOffset, double yMaxOffset);
    
    Point3D getLocation();
    
    /*
     This function sets up and inserts a rectangular area light source
     with size (width, height)
     orientation given by the normal vector (normal) and positive x unit vector (xUnit)
     centered at (centre)
     consisting of (numHori X numVert) point light sources (uniformly sampled)
     and with (colour) - which also determines intensity
     
     Note that the light source is visible as a uniformly colored rectangle.
     */
    static void addAreaLight(double width, double height, const Point3D &normal,
                             const Point3D &xUnit, const Point3D &centre,
                             int numHori, int numVert, const ColourRGB &colour,
                             list<Light*> &lights, list<Object3D*> *objects = NULL);
};

#endif /* AreaLightElement_h */
