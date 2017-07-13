#include "Sphere.h"
#include <math.h>

Sphere::Sphere(const Material &material, const ColourRGB &colour) :
Object3D::Object3D(material, colour) {
    Object3D::isLight = false;
}

ColourRGB Sphere::colourAtLocalPoint(const Point3D &p) const {
    // If no texture, return this object's standard color
    if (textureImage.rgbImageData == NULL) {
        return colour;
    }
    
    double theta = acos(p.z / radius);
    double phi = atan(p.y / p.x);
    double a = fmod(phi, 2*M_PI) / (2*M_PI);
    double b = (M_PI - theta) / M_PI;
    return textureImage.textureMap(a, b);
}

Intersection Sphere::intersect(const Ray3D &ray) {
    Intersection intersection;
    
    /////////////////////////////////
    // TO DO: Complete this function.
    /////////////////////////////////
    
    // Acquire the ray in local coordinates
    Point3D rayOrigin = invTransform * ray.origin;
    Point3D rayDirection = invTransform * ray.direction;
    
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

bool Sphere::doesIntersect(const Ray3D &ray) {
    return !intersect(ray).none;
}
