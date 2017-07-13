#include "TriangleMesh.h"
#include <vector>
#include <algorithm>
#include <string>

TriangleMesh::TriangleMesh(const vector<Material> &materials, const vector<ColourRGB> &colours) :
Object3D::Object3D(Material(), ColourRGB()){
    Object3D::isLight = false;
    this->materials = materials;
    this->colours = colours;
    for (int i = 0; i < materials.size(); i++) {
        textureImages.push_back(Image());
    }
}

TriangleMesh::TriangleMesh(const string filename, const vector<Material> &materials, const vector<ColourRGB> &colours) :
Object3D::Object3D(Material(), ColourRGB()){
    Object3D::isLight = false;
    this->materials = materials;
    this->colours = colours;
    for (int i = 0; i < materials.size(); i++) {
        textureImages.push_back(Image());
    }
    
	bool isLoaded = loadOBJ(filename);
	if (isLoaded)
	{
		printf("OBJ successfully loaded!\n");
	}
	else{
		printf("Error: OBJ NOT LOADED!\n");
	}
}

ColourRGB TriangleMesh::colourAtTrianglePoint(int mesh, TriangleFace *face, double u, double v) const {
    if (textureImages[mesh].rgbImageData == NULL) {
    	return colours[mesh];
    }
    // Find texture coordinates at every vertex on the triangle face
    int v1 = face->vertices[0];
    int v2 = face->vertices[1];
    int v3 = face->vertices[2];
    double a1 = meshes[mesh].Vertices[v1].TextureCoordinate.X;
    double b1 = meshes[mesh].Vertices[v1].TextureCoordinate.Y;
    double a2 = meshes[mesh].Vertices[v2].TextureCoordinate.X;
    double b2 = meshes[mesh].Vertices[v2].TextureCoordinate.Y;
    double a3 = meshes[mesh].Vertices[v3].TextureCoordinate.X;
    double b3 = meshes[mesh].Vertices[v3].TextureCoordinate.Y;
    
    // Interpolate texture coordinates and get the corresponding colour
    double a = a1*(1-u-v) + a2*u + a3*v;
    double b = b1*(1-u-v) + b2*u + b3*v;
    return textureImages[mesh].textureMap(a, b);
}

void TriangleMesh::loadTextureForMesh(int i, const char *filename) {
    bool success = Image::readPPMimage(filename, &textureImages[i]);
    if (!success) {
        printf("Error: could not load texture %s\n", filename);
    }
}

bool TriangleMesh::loadOBJ(const string path)
{
	//Initialize loader
	objl::Loader Loader;

	//Load obj
	bool loadout = Loader.LoadFile(path);

	if (loadout)
	{
        isLoaded = true;
        
        // Create/Open e1Out.txt
		std::ofstream file("e1Out.txt");

		// Copy one of the loaded meshes to be our current mesh
        for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
            meshes.push_back(Loader.LoadedMeshes[i]);
            if (meshes[i].MeshMaterial.name != "") {
                // Diffuse rgb components of mesh material should be
                // the colour of the mesh material.
                // All phong components should be the average of
                // their mesh rgb components. The shinyness as well.
                // The opacity and index of refraction
                objl::Material &mat = meshes[i].MeshMaterial;
                colours[i] = ColourRGB(mat.Kd.X, mat.Kd.Y, mat.Kd.Z);
                materials[i].ambient = Point3D(mat.Ka, true).average();
                materials[i].diffuse = Point3D(mat.Kd, true).average();
                materials[i].specular = Point3D(mat.Ks, true).average();
                materials[i].shinyness = mat.Ns;
            }
        }

		normalizeVertices();
		// Print Mesh Name
        for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
            file << "Mesh " << 0 << ": " << meshes[i].MeshName << "\n";
        }

		// Leave a space to separate from the next mesh
		file << "\n";

		// Close File
		file.close();
        
        // Construct a bounding box for every mesh
        for (int i = 0; i < meshes.size(); i++) {
            boundingBoxes.push_back(BoundingBox::BuildVolumeHierarchy(meshes[i]));
        }
	}
	// If not output an error
	else
	{
		// Create/Open e1Out.txt
		std::ofstream file("e1Out.txt");

		// Output Error
		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";

		// Close File
		file.close();
	}
    
	return loadout;
}

Intersection TriangleMesh::intersect(const Ray3D &ray) {
	Intersection intersection;

    // Acquire ray in local coordinates
    Point3D rayOrigin = invTransform*ray.origin; //e
    Point3D rayDirection = invTransform*ray.direction; //d
    
    // Go through triangle faces and keep track
    // of the one with the closest intersection
    TriangleFace *face = NULL;
    double lambda = DBL_MAX;
    int mesh = 0;
    double u = 0, v = 0;    // u,v parameters for parametric equation of triangle
    for (int i = 0; i < meshes.size(); i++) {
        double meshU, meshV, meshLambda;
        TriangleFace *meshFace = boundingBoxes[i].intersect(rayOrigin, rayDirection,
                                                            &meshLambda, &meshU, &meshV);
        if (meshLambda < lambda) {
            face = meshFace;
            lambda = meshLambda;
            u = meshU;
            v = meshV;
            mesh = i;
        }
    }

    if (lambda == DBL_MAX)
    {
        intersection.none = true;
        return intersection;
    }

    Point3D normal = findNormal(mesh, face, u, v);
    Point3D hitNormalLocal;
    bool insideObject, canSelfReflect;
    if (rayDirection.dot(normal) < 0) { // outside object
        hitNormalLocal = normal;
        insideObject = false;
        canSelfReflect = false;
    }
    else {                              // inside object
        hitNormalLocal = -1*normal;
        insideObject = true;
        canSelfReflect = true;
    }
    
    intersection.none = false;
    intersection.isLight = Object3D::isLight;
    intersection.insideObject = insideObject;
    intersection.lambda = lambda;
    intersection.point = ray.rayPosition(lambda);
    intersection.normal = (invTransform.transpose() * hitNormalLocal).normalized();
    intersection.material = materials[mesh];
    intersection.colour = colourAtTrianglePoint(mesh, face, u, v);
    intersection.canSelfReflect = canSelfReflect;
    intersection.obj = this;
    
    return intersection;
}

bool TriangleMesh::doesIntersect(const Ray3D &ray) {
    // Acquire ray in local coordinates
    Point3D rayOrigin = invTransform*ray.origin; //e
    Point3D rayDirection = invTransform*ray.direction; //d
    
    // Go through triangle faces and return true as soon as the
    // ray intersects one of them
    for (int i = 0; i < boundingBoxes.size(); i++) {
        if (boundingBoxes[i].doesIntersect(rayOrigin, rayDirection)) return true;
    }
    
    return false;
}

Point3D TriangleMesh::findNormal(int mesh, TriangleFace *face, double u, double v) {
    // Find normals of each vertex on the face
    int v1 = face->vertices[0];
    int v2 = face->vertices[1];
    int v3 = face->vertices[2];
	Point3D n1 = Point3D(meshes[mesh].Vertices[v1].Normal, true).normalized();
	Point3D n2 = Point3D(meshes[mesh].Vertices[v2].Normal, true).normalized();
	Point3D n3 = Point3D(meshes[mesh].Vertices[v3].Normal, true).normalized();
    
    // Interpolate to get actual normal based on u and v
    return (1-u-v)*n1 + u*n2 + v*n3;
}

void TriangleMesh::normalizeVertices(void) {
	//Normalize vertex coords to between -0.5 to 0.5
    double maxX = -DBL_MAX, minX = DBL_MAX;
    double maxY = -DBL_MAX, minY = DBL_MAX;
    double maxZ = -DBL_MAX, minZ = DBL_MAX;
    for (int i = 0; i < meshes.size(); i++) {
        maxX = max(meshes[i].max_x, maxX);
        minX = min(meshes[i].min_x, minX);
        maxY = max(meshes[i].max_y, maxY);
        minY = min(meshes[i].min_y, minY);
        maxZ = max(meshes[i].max_z, maxZ);
        minZ = min(meshes[i].min_z, minZ);
    }
    double scale = min(min(1.0/(maxX-minX), 1.0/(maxY-minY)), 1.0/(maxZ-minZ));
    
    for (int i = 0; i < meshes.size(); i++) {
        for (int j = 0; j < meshes[i].Vertices.size(); j++)
        {
            double x = meshes[i].Vertices[j].Position.X;
            double y = meshes[i].Vertices[j].Position.Y;
            double z = meshes[i].Vertices[j].Position.Z;
            
            meshes[i].Vertices[j].Position.X = (x - 0.5*(minX+maxX))*scale;
            meshes[i].Vertices[j].Position.Y = (y - 0.5*(minY+maxY))*scale;
            meshes[i].Vertices[j].Position.Z = (z - 0.5*(minZ+maxZ))*scale;
        }
    }
}
