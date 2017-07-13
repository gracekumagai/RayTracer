#ifndef TriangleMesh_h
#define TriangleMesh_h

#include "OBJ_Loader.h"

#include "../Object3D.h"
#include "BoundingBox.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>

using namespace std;

class TriangleMesh : public Object3D{
	/*
	 * Input: str path to .obj file
	 */
    
    bool isLoaded = false;
    vector<BoundingBox> boundingBoxes;
    vector<objl::Mesh> meshes;
    vector<ColourRGB> colours;
    vector<Material> materials;
    vector<Image> textureImages;
    
    void normalizeVertices(void);
    Point3D findNormal(int mesh, TriangleFace *face, double u, double v);
	ColourRGB colourAtTrianglePoint(int mesh, TriangleFace *face, double u, double v) const;

public:
	TriangleMesh(const vector<Material> &materials, const vector<ColourRGB> &colours);
	TriangleMesh(const string filename, const vector<Material> &materials, const vector<ColourRGB> &colours);
    
    void loadTextureForMesh(int i, const char *filename);

    bool loadOBJ(const string path);

	Intersection intersect(const Ray3D &ray);
    
    bool doesIntersect(const Ray3D &ray);
};

#endif
