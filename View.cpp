#include "View.h"

View::View(const Point3D &e, const Point3D &g, const Point3D &up,
     double f, double wsize) {
    // Setup camera position and basis vectors
    this->e = e;
    w = (-1*g).normalized();
    u = w.crossUnit(up);
    v = u.crossUnit(w);
    
    // Copy focal length and window size parameters
    this->f = f;
    this->wsize = wsize;
    
    // Setup coordinate conversion matrices
    cameraToWorld = Transform3D(u.x, v.x, w.x, e.x,
                                u.y, v.y, w.y, e.y,
                                u.z, v.z, w.z, e.z,
                                0.0, 0.0, 0.0, 1.0);
    worldToCamera = Transform3D(u.x, u.y, u.z, -u.dot(e),
                                v.x, v.y, v.z, -v.dot(e),
                                w.x, w.y, w.z, -w.dot(e),
                                0.0, 0.0, 0.0, 1.0);
}
