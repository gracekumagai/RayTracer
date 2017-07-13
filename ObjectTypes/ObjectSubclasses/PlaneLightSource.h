#ifndef PlaneLightSource_h
#define PlaneLightSource_h

#include "Plane.h"
#include <stdio.h>

class PlaneLightSource : public Plane {
public:
    PlaneLightSource(const ColourRGB &colour, const Point3D &centre,
                     const Point3D &xB, const Point3D &yB, const Point3D &zB,
                     double width, double height);
};

#endif /* PlaneLightSource_h */
