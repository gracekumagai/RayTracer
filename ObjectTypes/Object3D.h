#ifndef Object3D_h
#define Object3D_h

#include <stdio.h>
#include "Intersection.h"
#include "Image.h"
#include "Ray3D.h"
#include "../GeometricTypes/Transform3D.h"
#include "../GeometricTypes/AnimatedTransform.h"

struct Geometry
{
public:
	Point3D myPoint, myNormal;
	double myU, myV;

	const Object3D *myObject;
	Point3D myDpdu, myDpdv, myDndu, myDndv;

	Geometry();
	Geometry(const Point3D &p, const Point3D &dpdu, const Point3D &dpdv,
			 const Point3D &dndu, const Point3D &dndv, double u, double v,
			 const Object3D *object);
};

class Object3D {
protected:
    bool isLight;       // Flag to indicate if this object is a light
    
public:
    Material material;
    ColourRGB colour;
    Transform3D transform;      // Model -> World
    Transform3D invTransform;   // World -> Model
	AnimatedTransform *animTransform;
    
    Image textureImage;         // Structure holding the texture for this object
	Image normalImage;			// Structure holding the normals for this object

    bool isLightSource();   // Scene objects can be lights
    
    Object3D(const Material &material, const ColourRGB &colour);
    Object3D(const Object3D &obj);
    virtual ~Object3D(){}
    
    Object3D& operator=(const Object3D &obj);
    
    // Functions to subclass for different object types
    // (e.g. plane, sphere, etc.)
    
    // Note that the intersection function must compute the lambda at the intersection, the
    // intersection point p, the normal at that point n, and the object color.
    // This has an empty implementation because it is supposed to be overriden by a subclass
    virtual Intersection intersect(const Ray3D &ray) = 0;
    
    // Determines if a ray does intersect an object anywhere
    virtual bool doesIntersect(const Ray3D &ray) = 0;
    
    // WARNING: These methods are mutable, meaning that they change the
    // data in the class. I don't like this style, but it is necessary
    // for efficiency
    void rotateX(double theta);
    void rotateY(double theta);
    void rotateZ(double theta);
    void translate(double x, double y, double z);
    void scale(double x, double y, double z);
	void updateInverse();

    Point3D originInWorld() const;
    
	bool useNormalMap();

    // Load a texture image from file and assign it to this object
    void loadTexture(const char *filename);

	void loadNormalTexture(const char *filename);
};

#endif
