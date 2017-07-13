#include "BoundingBox.h"
#include <algorithm>
#include <unordered_map>

TriangleFace::TriangleFace(int v1, int v2, int v3, const objl::Mesh &mesh) {
    vertices[0] = v1; vertices[1] = v2; vertices[2] = v3;
    maxX = -DBL_MAX; minX = DBL_MAX;
    maxY = -DBL_MAX; minY = DBL_MAX;
    maxZ = -DBL_MAX; minZ = DBL_MAX;
    for (int i = 0; i < 3; i++) {
        maxX = fmax(maxX, mesh.Vertices[vertices[i]].Position.X);
        minX = fmin(minX, mesh.Vertices[vertices[i]].Position.X);
        maxY = fmax(maxY, mesh.Vertices[vertices[i]].Position.Y);
        minY = fmin(minY, mesh.Vertices[vertices[i]].Position.Y);
        maxZ = fmax(maxZ, mesh.Vertices[vertices[i]].Position.Z);
        minZ = fmin(minZ, mesh.Vertices[vertices[i]].Position.Z);
    }
    
    // Compute intersection transform
    Point3D p1(mesh.Vertices[v1].Position.X, mesh.Vertices[v1].Position.Y, mesh.Vertices[v1].Position.Z, false);
    Point3D p2(mesh.Vertices[v2].Position.X, mesh.Vertices[v2].Position.Y, mesh.Vertices[v2].Position.Z, false);
    Point3D p3(mesh.Vertices[v3].Position.X, mesh.Vertices[v3].Position.Y, mesh.Vertices[v3].Position.Z, false);
    
    Point3D ab = p2 - p1;
    Point3D ac = p3 - p1;
    Point3D n = ab.crossUnit(ac);
    
    intersectionTransform = Transform3D(ab, ac, n, p1).inverse();
}

TriangleFace::TriangleFace(const TriangleFace &other) {
    vertices[0] = other.vertices[0]; vertices[1] = other.vertices[1]; vertices[2] = other.vertices[2];
    maxX = other.maxX; maxY = other.maxY; maxZ = other.maxZ;
    minX = other.minX; minY = other.minY; minZ = other.minZ;
    intersectionTransform = other.intersectionTransform;
}

double TriangleFace::findIntersectionParams(Point3D &origin, Point3D &direction,
                                            double *u, double *v) {
    Point3D o = intersectionTransform * origin;
    Point3D d = intersectionTransform * direction;
    
    if (fabs(d.z) < 1e-6) return DBL_MAX;
    
    double t = -o.z / d.z;
    *u = o.x + t * d.x;
    *v = o.y + t * d.y;
    
    if (t < 0 || *u < 0 || *v < 0 || *u+*v > 1) return DBL_MAX;
    
    return t;
}

BoundingBox BoundingBox::BuildVolumeHierarchy(const objl::Mesh &mesh) {
    // Construct all initial faces that will be in the bouding box
    vector<TriangleFace> faces;
    for (int i = 0; i < mesh.Indices.size(); i += 3) {
        faces.push_back(TriangleFace(mesh.Indices[i],
                                     mesh.Indices[i+1],
                                     mesh.Indices[i+2],
                                     mesh));
    }
    
    return BoundingBox(faces, 0, mesh, maxDepth);
}

BoundingBox::BoundingBox(vector<TriangleFace> faces, int splitAxis, const objl::Mesh &mesh, int depth) {
    findBoundsForFaces(faces);   // set the bounds of the bounding box based on faces enclosed
    
    // Base case, don't reduce bounding box any more (don't give it any more children)
    if (faces.size() <= 5 || depth == 0) {
        this->faces = faces;
        return;
    }
    
    // Sort all vertices for the faces of this bounding box
    // along the split axis. Do not add duplicates
    unordered_map<int, bool> found;
    vector<int> vertices;
    for (int i = 0; i < faces.size(); i++) {
        if (found.find(faces[i].vertices[0]) == found.end()) {
            vertices.push_back(faces[i].vertices[0]);
            //found[faces[i].vertices[0]] = true;
            found.insert(pair<int, bool>(faces[i].vertices[0], true));
        }
        if (found.find(faces[i].vertices[1]) == found.end()) {
            vertices.push_back(faces[i].vertices[1]);
            found[faces[i].vertices[1]] = true;
        }
        if (found.find(faces[i].vertices[2]) == found.end()) {
            vertices.push_back(faces[i].vertices[2]);
            found[faces[i].vertices[2]] = true;
        }
    }
    sort(vertices.begin(), vertices.end(), [splitAxis, &mesh](const int &v1, const int &v2){
        switch (splitAxis) {
            case 0:
                return mesh.Vertices[v1].Position.X < mesh.Vertices[v2].Position.X;
            case 1:
                return mesh.Vertices[v1].Position.Y < mesh.Vertices[v2].Position.Y;
            case 2:
                return mesh.Vertices[v1].Position.Z < mesh.Vertices[v2].Position.Z;
            default:
                return false;
        }
        
    });
    
    // Find the coordinate along the split axis
    // of the median so that we can split our faces
    // roughly evenly
    unsigned long medianIndex = vertices.size()/2;
    objl::Vector3 medianPos = mesh.Vertices[vertices[medianIndex]].Position;
    double medianCoord;
    switch (splitAxis) {
        case 0:
            medianCoord = medianPos.X;
            break;
        case 1:
            medianCoord = medianPos.Y;
            break;
        case 2:
            medianCoord = medianPos.Z;
            break;
        default:
            medianCoord = 0;
            break;
    }
    
    // Put all faces with a vertex before the split
    // point in one child bounding box, and all the faces
    // with a vertex after the split point in another child.
    // NOTE: faces may end up being in 2 children
    vector<TriangleFace> minBoxFaces, maxBoxFaces;
    for (int i = 0; i < faces.size(); i++) {
        double maxOnAxis; double minOnAxis;
        switch (splitAxis) {
            case 0:
                maxOnAxis = faces[i].maxX;
                minOnAxis = faces[i].minX;
                break;
                break;
            case 1:
                maxOnAxis = faces[i].maxY;
                minOnAxis = faces[i].minY;
                break;
            case 2:
                maxOnAxis = faces[i].maxZ;
                minOnAxis = faces[i].minZ;
                break;
            default:
                maxOnAxis = 0;
                minOnAxis = 0;
                break;
        }
        
        if (maxOnAxis >= medianCoord)
            maxBoxFaces.push_back(faces[i]);
        if (minOnAxis <= medianCoord) {
            minBoxFaces.push_back(faces[i]);
        }
    }
    
    splitAxis = (splitAxis+1)%3;    // alternate the split axis
    depth--;
    children.push_back(BoundingBox(minBoxFaces, splitAxis, mesh, depth));
    children.push_back(BoundingBox(maxBoxFaces, splitAxis, mesh, depth));
}

TriangleFace* BoundingBox::intersect(Point3D &origin, Point3D &direction, double *lambda, double *u, double *v) {
    if (!doesIntersectBox(origin, direction)) {
        *lambda = DBL_MAX;
        return NULL;
    }
    
    // Closest of bounding box vertices including all its children
    TriangleFace *closestFace = NULL;
    double closestLambda = DBL_MAX;
    double closestU = 0, closestV = 0;
    
    // Find the closest intersection of all your own vertices
    for (int i = 0; i < faces.size(); i++) {
        double uFace = 0, vFace = 0;
        double lambdaFace = faces[i].findIntersectionParams(origin, direction, &uFace, &vFace);
        if (lambdaFace < closestLambda) {
            closestFace = &faces[i];
            closestLambda = lambdaFace;
            closestU = uFace;
            closestV = vFace;
        }
    }
    
    // Find the  closest intersection of your children
    for (int i = 0; i < children.size(); i++) {
        double uChild = 0, vChild = 0, lambdaChild = 0;
        TriangleFace *faceChild;
        faceChild = children[i].intersect(origin, direction, &lambdaChild, &uChild, &vChild);
        if (lambdaChild < closestLambda) {
            closestFace = faceChild;
            closestLambda = lambdaChild;
            closestU = uChild;
            closestV = vChild;
        }
    }
    
    *lambda = closestLambda;
    *u = closestU;
    *v = closestV;
    return closestFace;
}

bool BoundingBox::doesIntersect(Point3D &origin, Point3D &direction) {
    if (!doesIntersectBox(origin, direction)) return false;
    
    // Check through your own faces
    for (int i = 0; i < faces.size(); i++) {
        double u, v;
        double lambda = faces[i].findIntersectionParams(origin, direction, &u, &v);
        if (lambda < DBL_MAX) return true;
    }
    
    // Check through your children
    for (int i = 0; i < children.size(); i++) {
        if (children[i].doesIntersect(origin, direction)) return true;
    }
    
    return false;
}

void BoundingBox::findBoundsForFaces(const vector<TriangleFace> containedFaces) {
    maxX = -DBL_MAX; minX = DBL_MAX;
    maxY = -DBL_MAX; minY = DBL_MAX;
    maxZ = -DBL_MAX; minZ = DBL_MAX;
    for (int i = 0; i < containedFaces.size(); i++) {
        maxX = fmax(maxX, containedFaces[i].maxX);
        minX = fmin(minX, containedFaces[i].minX);
        maxY = fmax(maxY, containedFaces[i].maxY);
        minY = fmin(minY, containedFaces[i].minY);
        maxZ = fmax(maxZ, containedFaces[i].maxZ);
        minZ = fmin(minZ, containedFaces[i].minZ);
    }
}

bool BoundingBox::doesIntersectBox(const Point3D &origin, const Point3D &direction) {
    double tmin = -DBL_MAX, tmax = DBL_MAX;
    Point3D bmin(minX, minY, minZ, true);
    Point3D bmax(maxX, maxY, maxZ, true);
    
    if (direction.x != 0) {
        double tx1 = (bmin.x - origin.x) / direction.x;
        double tx2 = (bmax.x - origin.x) / direction.x;
        tmin = max(tmin, min(tx1, tx2));
        tmax = min(tmax, max(tx1, tx2));
    }
    if (direction.y != 0) {
        double ty1 = (bmin.y - origin.y) / direction.y;
        double ty2 = (bmax.y - origin.y) / direction.y;
        tmin = max(tmin, min(ty1, ty2));
        tmax = min(tmax, max(ty1, ty2));
    }
    if (direction.z != 0) {
        double tz1 = (bmin.z - origin.z) / direction.z;
        double tz2 = (bmax.z - origin.z) / direction.z;
        tmin = max(tmin, min(tz1, tz2));
        tmax = min(tmax, max(tz1, tz2));
    }
    
    // Check if box is behind
    if (tmax < 0) return false;
    
    return tmax >= tmin;
}
