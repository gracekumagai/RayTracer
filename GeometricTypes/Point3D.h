#ifndef Point3D_h
#define Point3D_h

#include "../ObjectTypes/ObjectSubclasses/OBJ_Loader.h"


// forward declare the object type to avoid circular dependencies
class Object3D;

class Point3D {
    double w;
    
public:
    double x, y, z;
    
    Point3D();
    Point3D(double x, double y, double z, bool isVec);
    Point3D(objl::Vector3 &v, bool isVec);
    Point3D(Point3D &p1, Point3D &p2, Point3D &p3, bool isVec);

    // default copy constructor suffices
    
    double magnitude() const;
    Point3D normalized() const;
    
    double dot(const Point3D &other) const;
    Point3D crossUnit(const Point3D &other) const;    // normalized cross product
    
    Point3D operator+(const Point3D &other) const;
    Point3D operator-(const Point3D &other) const;
    Point3D operator*(double scale) const;          // vec * scalar
    
    bool operator==(const Point3D &other) const;
    bool operator!=(const Point3D &other) const;
    
    Point3D linearInterpolate(const Point3D &end, double progress) const; // progress: [0,1]
    
    // Randomly perturb the direction of a vector within the bouds of 'degree' degrees on a cone.
    // 'normal' is used to make a basis with the current vector
    // 'degree' in bounds 0 to 1
    // Return value will have unit length
    Point3D randomlyPerturb(const Point3D &normal, double degree) const;
    
    // debugging
    void printPoint3D() const;
    
    // Probably won't be used much because w should always be 0 or 1
    // and the programmer will know which one. Will likely only be used
    // during matrix multiplication with a non-affine matrix
    Point3D(double x, double y, double z, double w);
    Point3D homogeonized() const;
    double average();

};

Point3D operator*(double scale, const Point3D &p); // scalar * vec

#endif
