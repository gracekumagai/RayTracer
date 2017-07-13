#include "PlaneLightSource.h"

PlaneLightSource::PlaneLightSource(const ColourRGB &colour, const Point3D &centre,
                                   const Point3D &xB, const Point3D &yB, const Point3D &zB,
                                   double width, double height) :
Plane::Plane(Material(), colour) {
    Object3D::isLight = true;
    
    transform = Transform3D(xB*width, yB*height, zB, centre);
    updateInverse();
}
