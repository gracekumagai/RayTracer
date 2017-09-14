#ifndef View_h
#define View_h

#include <stdio.h>
#include "GeometricTypes/Transform3D.h"

class View 
{
public:
    Point3D myE;		// location of the camera center
    Point3D myU;		// u vector
    Point3D myV;		// v vector
    Point3D myW;		// w vector
    
    double myFocalLength;		// focal length
    double myWindowSize;   // window size in distance units (not pixels!)
    
    Transform3D myWorldToCamera;
    Transform3D myCameraToWorld;
    
    /*
     This function sets up the camera axes and viewing direction:
     e - Camera center
     g - Gaze direction
     up - Up vector
     fov - Field of view in degrees
     f - focal length
     */
    View(const Point3D &e, const Point3D &g, const Point3D &up,
         double f, double wsize);
};

#endif
