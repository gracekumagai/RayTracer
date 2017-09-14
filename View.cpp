#include "View.h"

View::View(const Point3D &e, const Point3D &g, const Point3D &up,
     double f, double wsize) 
{
    // Setup camera position and basis vectors
    this->myE = e;
    myW = (-1*g).normalized();
    myU = myW.crossUnit(up);
    myV = myU.crossUnit(myW);
    
    // Copy focal length and window size parameters
    this->myFocalLength = f;
    this->myWindowSize = wsize;
    
    // Setup coordinate conversion matrices
    myCameraToWorld = Transform3D(myU.x, myV.x, myW.x, e.x,
								  myU.y, myV.y, myW.y, e.y,
                                  myU.z, myV.z, myW.z, e.z,
                                  0.0, 0.0, 0.0, 1.0);
    myWorldToCamera = Transform3D(myU.x, myU.y, myU.z, -myU.dot(e),
                                  myV.x, myV.y, myV.z, -myV.dot(e),
                                  myW.x, myW.y, myW.z, -myW.dot(e),
                                  0.0, 0.0, 0.0, 1.0);
}
