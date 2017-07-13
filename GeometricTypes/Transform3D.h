#ifndef Transform3D_h
#define Transform3D_h

#include "Point3D.h"

class Transform3D {
    double m[4][4];
    
public:
    static Transform3D identity();
    static Transform3D rotatedX(double theta);
    static Transform3D rotatedY(double theta);
    static Transform3D rotatedZ(double theta);
    static Transform3D translated(double x, double y, double z);
    static Transform3D scaled(double x, double y, double z);
    
    Transform3D();
    Transform3D(double r0c0, double r0c1, double r0c2, double r0c3,
                double r1c0, double r1c1, double r1c2, double r1c3,
                double r2c0, double r2c1, double r2c2, double r2c3,
                double r3c0, double r3c1, double r3c2, double r3c3);
    Transform3D(const Point3D &a, const Point3D &b, const Point3D &c, const Point3D &d);
    Transform3D(const Transform3D &t);
    
    Transform3D& operator=(const Transform3D &t);
    
    // NOTE: only to be used on affine matrices
    Transform3D inverse();
    Transform3D transpose() const;
    
    Transform3D operator*(double scale) const;  // matrix * scale
    Point3D operator*(const Point3D &p) const;     // matrix * vec
    Transform3D operator*(const Transform3D &t) const; // matrix * matrix
    
    Point3D translationComponent() const;
    
    void printTransform3D();
};

Transform3D operator*(double scale, const Transform3D &t);  // scalar * matrix

#endif
