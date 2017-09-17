#include "Sphere.h"
#include "../../Utilities/Math.h"
#include <math.h>

#define PI 3.14159265354

Sphere::Sphere(const Material &material, const ColourRGB &colour) :
Object3D::Object3D(material, colour) 
{
    Object3D::isLight = false;
	thetaMin = 0.0;
	thetaMin = PI;
	phiMax = 2.0 * PI;
}

Sphere::Sphere(double r, double z0, double z1, double pMax, const Material &material, const ColourRGB &colour) :
	Object3D::Object3D(material, colour)
{
	Object3D::isLight = false;
	zMin = Utility::Clamp(Utility::min(z0, z1), -r, r);
	zMax = Utility::Clamp(Utility::max(z0, z1), -r, r);
	thetaMin = acos(Utility::Clamp(zMin / r, -1.0, 1.0));
	thetaMax = acos(Utility::Clamp(zMax / r, -1.0, 1.0));
	phiMax = radians(Utility::Clamp(pMax, 0.0, 360.0));
}

ColourRGB Sphere::colourAtLocalPoint(const Point3D &p) const {
    // If no texture, return this object's standard color
    if (textureImage.myRGBImageData == NULL) {
        return colour;
    }
    
    double theta = acos(p.z / radius);
    double phi = atan(p.y / p.x);
    double a = fmod(phi, 2*PI) / (2*PI);
    double b = (PI - theta) / PI;
	
	if (a < 0.0 || b < 0.0)
		return ColourRGB(0.7, 0.1, 0.1);

	/*if ((sin(PI * p.x / 0.05) > 0))
	{
		//double t = (1 + sin(PI * p.y)) / 2.0;
		//return ColourRGB(0.5, 0.6, 0.9) * (1.0 - t) + ColourRGB(0.7, 0.3, 0.8) * t;
		if (sin(PI * p.y) > 0)
		{
			return ColourRGB(0.5, 0.6, 0.9);
		}
		return ColourRGB(0.7, 0.3, 0.8);
	}
	return ColourRGB(0.3, 0.3, 1.0);*/
	
    return textureImage.textureMap(a, b);
}

Intersection Sphere::intersect(const Ray3D &ray) {
    Intersection intersection;
    
    /////////////////////////////////
    // TO DO: Complete this function.
    /////////////////////////////////
    
    // Acquire the ray in local coordinates
    Point3D rayOrigin = invTransform * ray.myOrigin;
    Point3D rayDirection = invTransform * ray.myDirection;
    
    // Compute the components of the quadratic equation
    Point3D centreToRayOrigin = rayOrigin - centre;
    double a = rayDirection.dot(rayDirection);
    double b = 2 * rayDirection.dot(centreToRayOrigin);
    double c = centreToRayOrigin.dot(centreToRayOrigin) - radius*radius;
    
    // Compute the determinant of the quadratic equation
    double det = b*b - 4*a*c;
    
    // If determinant is negative, we have no solutions
    if (det < 0) {
        intersection.none = true;
        return intersection;
    }
    
    // Otherwise, compute the two solutions.
    // We do not consider the case when det = 0 because realistically
    // this will never happen. If it did, we just compute the same thing twice.
    double lambda1 = (-b + sqrt(det)) / (2 * a);
    double lambda2 = (-b - sqrt(det)) / (2 * a);
    
    // Compute the intersection point and normal
    bool insideObject, canSelfReflect;
    double lambda;
    Point3D hitPointLocal;
    Point3D hitNormalLocal;
    if (lambda1 < 0 && lambda2 < 0) {   // sphere behind us
        intersection.none = true;
        return intersection;
    }
    else if (lambda1 <= 0 || lambda2 <= 0) {  // inside sphere
        lambda = lambda1 < lambda2 ? lambda2 : lambda1;
        hitPointLocal = rayOrigin + lambda*rayDirection;
        hitNormalLocal = centre - hitPointLocal;
        insideObject = true;
        canSelfReflect = true;
    }
    else {                              // sphere in front of us
        lambda = lambda1 < lambda2 ? lambda1 : lambda2;
        hitPointLocal = rayOrigin + lambda*rayDirection;
        hitNormalLocal = hitPointLocal - centre;
        insideObject = false;
        canSelfReflect = false;
    }
    
    intersection.none = false;
    intersection.isLight = Object3D::isLight;
    intersection.insideObject = insideObject;
    intersection.lambda = lambda;
    intersection.point = ray.rayPosition(lambda);   // lambda is the same for local and world
    intersection.normal = (invTransform.transpose() * hitNormalLocal).normalized();
    intersection.material = material;
    intersection.colour = colourAtLocalPoint(hitPointLocal);
    intersection.canSelfReflect = canSelfReflect;
    intersection.obj = this;
    
    return intersection;
}

bool Sphere::doesIntersect(const Ray3D &ray) 
{
    return !intersect(ray).none;
}

double Sphere::area() const
{
	return phiMax * radius * (zMax - zMin);
}