#ifndef View_h
#define View_h

#include <stdio.h>
#include "GeometricTypes/Transform3D.h"

class View {
public:
    Point3D e;  // location of the camera center
    Point3D u;  // u vector
    Point3D v;  // v vector
    Point3D w;  // w vector
    
    double f;   // focal length
    double wsize;   // window size in distance units (not pixels!)
    
    Transform3D worldToCamera;
    Transform3D cameraToWorld;
    
    /*
     This function sets up the camera axes and viewing direction:
     e - Camera center
     g - Gaze direction
     up - Up vector
     fov - Fild of view in degrees
     f - focal length
     */
    View(const Point3D &e, const Point3D &g, const Point3D &up,
         double f, double wsize);
};

#endif
