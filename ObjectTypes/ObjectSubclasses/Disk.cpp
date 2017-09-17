#include "Disk.h"
#include "../../Utilities/Math.h"
#include <math.h>

Disk::Disk(const Material &material, const ColourRGB &colour) :
Object3D::Object3D(material, colour) 
{
    Object3D::isLight = false;
}

Disk::Disk(double height, double r, double ri, double t, const Material &material, const ColourRGB &colour) :
	Object3D::Object3D(material, colour)
{
	Object3D::isLight = false;
	myCenter.y = height;
	myRadius = r;
	myInnerRadius = ri;
	myPhiMax = radians(Utility::Clamp(t, 0.0, 360.0));
}

ColourRGB Disk::colourAtLocalPoint(const Point3D &p) const {
    // If no texture, return this object's standard color
    if (textureImage.myRGBImageData == NULL) {
        return colour;
    }
    
	double phi = atan2(p.z, p.x);
	if (phi < 0)		// Ensure range: [0, 2*PI]
	{
		phi += 2.0 * PI;
	}

	double dist = p.x * p.x + p.z * p.z;

	double a = phi / myPhiMax;
	double b = 1.0 - ((sqrt(dist) - myInnerRadius) / (myRadius - myInnerRadius));
	
    return textureImage.textureMap(a, b);
}

Intersection Disk::intersect(const Ray3D &ray) {
    Intersection intersection;
    
    /////////////////////////////////
    // TO DO: Complete this function.
    /////////////////////////////////
    
    // Acquire the ray in local coordinates
    Point3D rayOrigin = invTransform * ray.myOrigin;
    Point3D rayDirection = invTransform * ray.myDirection;
	Ray3D rayP = Ray3D(rayOrigin, rayDirection);

	// Reject disk intersections for rays || to disk's plane
	if (rayDirection.y == 0)
	{
		intersection.none = true;
		return intersection;
	}

	double lambda = (myCenter.y - rayOrigin.y) / rayDirection.y;

	// See if intersect inside disk radius and phimax
	Point3D hitPointLocal = rayP.rayPosition(lambda);
	
	double dist = hitPointLocal.x * hitPointLocal.x + hitPointLocal.z * hitPointLocal.z;
	if (dist > myRadius * myRadius || dist < myInnerRadius * myInnerRadius)
	{
		intersection.none = true;
		return intersection;
	}

	// Check phimax
	double phi = atan2(hitPointLocal.z, hitPointLocal.x);
	
	if (phi < 0.0)		// Ensure range: [0, 2*PI]
	{
		phi += 2 * PI;
	}
	if (phi > myPhiMax)	// Does not intersect with partial disk
	{
		intersection.none = true;
		return intersection;
	}
	Point3D hitNormalLocal = hitPointLocal - myCenter;

    intersection.none = false;
    intersection.isLight = Object3D::isLight;
    intersection.insideObject = false;
    intersection.lambda = lambda;
    intersection.point = ray.rayPosition(lambda);   // lambda is the same for local and world
    intersection.normal = (invTransform.transpose() * hitNormalLocal).normalized();
    intersection.material = material;
    intersection.colour = colourAtLocalPoint(hitPointLocal);
    intersection.canSelfReflect = false;
    intersection.obj = this;
    
    return intersection;
}

bool Disk::doesIntersect(const Ray3D &ray) 
{
    return !intersect(ray).none;
}

double Disk::area() const
{
	return myPhiMax * 0.5 * (myRadius * myRadius - myInnerRadius * myInnerRadius);
}