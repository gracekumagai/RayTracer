#ifndef BoundingBox_h
#define BoundingBox_h

#include "OBJ_Loader.h"
#include "../../GeometricTypes/Transform3D.h"
#include <vector>

using namespace std;

class TriangleFace {
public:
    int vertices[3];
    double maxX, minX, maxY, minY, maxZ, minZ;
    Transform3D intersectionTransform;
    
    TriangleFace(int v1, int v2, int v3, const objl::Mesh &mesh);
    TriangleFace(const TriangleFace &other);
    
    double findIntersectionParams(Point3D &origin, Point3D &direction,
                                  double *u, double *v);
};

class BoundingBox {
    static const int maxDepth = 10;
    
    void findBoundsForFaces(const vector<TriangleFace> containedFaces);
    bool doesIntersectBox(const Point3D &origin, const Point3D &direction);
    
public:
    vector<TriangleFace> faces;
    double maxX, minX, maxY, minY, maxZ, minZ;
    vector<BoundingBox> children;
    
    static BoundingBox BuildVolumeHierarchy(const objl::Mesh &mesh);
    BoundingBox(vector<TriangleFace> faces, int splitAxis, const objl::Mesh &mesh, int depth);
    
    TriangleFace* intersect(Point3D &origin, Point3D &direction, double *lambda, double *u, double *v);
    
    bool doesIntersect(Point3D &origin, Point3D &direction);
    
};

#endif /* BoundingBox_h */
