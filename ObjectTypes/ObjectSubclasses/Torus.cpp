#include "Torus.h"
#include "../../Utilities/Math.h"
#include <math.h>
#include <complex>

Torus::Torus(const Material &material, const ColourRGB &colour) :
Object3D::Object3D(material, colour) 
{
    Object3D::isLight = false;
}

Torus::Torus(Point3D center, Point3D axis, double major, double minor, const Material &material, const ColourRGB &colour) :
	Object3D::Object3D(material, colour)
{
	Object3D::isLight = false;
	myCenter = center;
	myAxis = axis.normalized();
	myMajorR = major;
	myMinorR = minor;
}

ColourRGB Torus::colourAtLocalPoint(const Point3D &p) const {
    // If no texture, return this object's standard color
    if (textureImage.myRGBImageData == NULL) {
        return colour;
    }
	double b = asin(abs(p.y / myMinorR)) / (2.0 * PI);
	double arg = p.x / (myMajorR + myMinorR * cos(b * 2.0 * PI));
	double a = (acos(arg)) / (2.0 * PI);

	return textureImage.textureMap(a, b);
}

Intersection Torus::intersect(const Ray3D &ray) {
    Intersection intersection;
    
    // Acquire the ray in local coordinates
    Point3D rayOrigin = invTransform * ray.myOrigin;
    Point3D rayDirection = (invTransform * ray.myDirection).normalized();

	// Useful constants
	Point3D Q = rayOrigin - myCenter;
	double u = myAxis.dot(Q);
	double v = myAxis.dot(rayDirection);

	double a = 1.0 - v * v;
	double b = 2.0 * (Q.dot(rayDirection) - u * v);
	double c = Q.dot(Q) - u * u;
	double d = Q.dot(Q) + myMajorR * myMajorR - myMinorR * myMinorR;

	// Find coefficients for quatric equation
	// where, Ax^4 + Bx^3 + Cx^2 + Dx^1 + E = 0
	double coefficients[5];

	coefficients[0] = 1.0;																				// A
	coefficients[1] = 4 * Q.dot(rayDirection);															// B
	coefficients[2] = 2 * d + 0.25 * coefficients[1] * coefficients[1] - 4.0 * myMajorR * myMajorR * a;	// C
	coefficients[3] = coefficients[1] * d - 4.0 * myMajorR * myMajorR * b;								// D
	coefficients[4] = d * d - 4.0 * myMajorR * myMajorR * c;											// E

	double roots[4] = {INFINITY, INFINITY, INFINITY, INFINITY};
	int numRoots = solveQuartic(coefficients, roots);
	int numPosRoots = findNumPositiveRoots(roots, numRoots);

	// No real roots or no positive roots - entire torus is behind the camera
	if (numRoots == 0 || numPosRoots == 0)
	{
		intersection.none = true;
		return intersection;
	}

	double lambda = roots[numRoots - numPosRoots];
	Point3D hitPointLocal = rayOrigin + lambda*rayDirection;

	bool insideObject, canSelfReflect;

	// Find Normal
	double y = (hitPointLocal - myCenter).dot(myAxis);
	Point3D direction = (hitPointLocal - myCenter) - y * myAxis;
	Point3D X = Point3D(0, 0, 0, true);
	if (direction.dot(direction) != 0)
		X = direction * (1 / sqrt(direction.dot(direction))) * myMajorR;

	Point3D hitNormalLocal = hitPointLocal - myCenter - X;
	
	// Determine if inside or outside torus
	if (numPosRoots % 2 == 0)
	{
		insideObject = false;
		canSelfReflect = true;
	}
	else
	{
		insideObject = true;
		canSelfReflect = true;
		hitNormalLocal = -1.0 * hitNormalLocal;
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

bool Torus::doesIntersect(const Ray3D &ray)
{
    return !intersect(ray).none;
}

double Torus::area() const
{
	return 0;
}