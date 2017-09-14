#include <list>
#include <time.h>
#include <float.h>
#include "RayTracer.h"
#include "Utilities/ProgressManager.h"
#include "Utilities/Math.h"
#include "ObjectTypes/PerlinNoise.h"

void RayTracer::renderImage(View camera, Scene scene,
                            Image *output, char * name, vector<int> bounds) 
{
    // Store local copies objects and lights to minimize their passing between functions
    this->objects = scene.myObjects;
    this->lights = scene.myLights;
	this->mySkyBox = scene.myProp.mySkybox;
    
    int rows = bounds[1] - bounds[0];
    int cols = bounds[3] - bounds[2];

    // Object to keep track of the progress and give us some feedback
    ProgressManager progressManager(rows * cols);
    progressManager.startTimer();

    // Itterate through all the pixels and do the ray tracing
#pragma omp parallel for schedule(dynamic)		// Multithread rendering
    for (int i = 0; i < output->myX; i++)
    {
        for (int j = 0; j < output->myY; j++)
        {
            if (i < bounds[0] || i >= bounds[1] || j < bounds[2] || j >= bounds[3]) 
			{
                output->setColourAtPixel(i, j, ColourRGB(0, 0, 0));
                continue;
            }
            
            // Find the current ray
            Point3D origin(0, 0, 0, false);
            double xIncrement = ((i+0.5)/output->myX) * camera.myWindowSize;
            double yIncrement = ((j+0.5)/output->myY) * camera.myWindowSize;
            Point3D targetPixel(camera.myWindowSize/2 - xIncrement,
                                camera.myWindowSize/2 - yIncrement,
                                camera.myFocalLength, false);
            Point3D direction = targetPixel - origin;
            Ray3D ray(camera.myCameraToWorld * origin,
                      camera.myCameraToWorld * direction);
            
            // Trace the pixel and store it in the image
            ColourRGB pixelColour = rayTrace(ray, camera.myWindowSize / output->myX, targetPixel);
            if (pixelColour.outOfBounds()) 
			{
                pixelColour.normalize();
            }
            output->setColourAtPixel(i, j, pixelColour);

            progressManager.advance();
        }
    }
    
    // Output rendered image
    output->outputImage(name);
}

ColourRGB RayTracer::rayTrace(Ray3D ray, double pixelSize, Point3D targetPixel) 
{
    ColourRGB pixelColour(0, 0, 0);
    
    if (myAntialiasingEnabled) 
	{
        double subPixelSize = pixelSize / mySuperSamplingResolution;
        for (int i = 0; i < mySuperSamplingResolution; i++) 
		{
            for (int j = 0; j < mySuperSamplingResolution; j++) 
			{
                // Compute a random x,y offset within the subPixelSize
                Point3D offset(drand48()*subPixelSize + i*subPixelSize - 0.5*pixelSize,
                               drand48()*subPixelSize + j*subPixelSize + 0.5*pixelSize,
                               0, true);
                
                // Move the ray by the random offset
                Ray3D subRay = ray.offset(offset);
                
                // Cast the ray. We divide by the number of super-samples we have
                // in order to compute the average colour over all rays for this pixel
                pixelColour += rayTraceRecursive(subRay, myMaxDepth) *
                        (1.0 / (mySuperSamplingResolution*mySuperSamplingResolution));
            }
        }
    }
    
	else if (myDOFEnabled) 
	{
		Point3D focalPoint = ray.rayPosition(myFocalLength);
		for (int i = 0; i < myDOFResolution; i++) 
		{
			float du = randDouble(-myB, myB);
			float dv = randDouble(-myB, myB);

			Point3D newOrigin = ray.myOrigin;
			newOrigin.x += du;
			newOrigin.y += dv;

			Point3D newDirection = focalPoint - newOrigin;
			Ray3D subRay = Ray3D(newOrigin, newDirection.normalized());
			pixelColour += rayTraceRecursive(subRay, myMaxDepth) * (1.0 / myDOFResolution);

		}
	}

    else {
        ray = ray.normalized();
        pixelColour = rayTraceRecursive(ray, myMaxDepth);
    }
    
    return pixelColour;
}

ColourRGB RayTracer::rayTraceRecursive(const Ray3D &ray, int depth, Object3D *excludedSource) 
{   
    Intersection firstHit = findFirstHit(ray, excludedSource);
    if (firstHit.none) {
        if (mySkyBox != NULL) {
            return mySkyBox->colourInDirection(ray.myDirection);
        }
        return ColourRGB(0, 0, 0);
    }
    else if (firstHit.isLight) {    // Lights just emmit their colour
        return firstHit.colour;
    }
    
	ColourRGB colour = shade(firstHit, ray, depth);

    return colour;
}

ColourRGB RayTracer::shade(Intersection &intersection, const Ray3D &ray, int depth) {
    ColourRGB res(0.0, 0.0, 0.0);
	ColourRGB refl(0.0, 0.0, 0.0);
	ColourRGB refract(0.0, 0.0, 0.0);

	ColourRGB resRed(0.0, 0.0, 0.0);
	ColourRGB resGreen(0.0, 0.0, 0.0);
	ColourRGB resBlue(0.0, 0.0, 0.0);
    
    res += phongModel(intersection, ray);
  
	if (depth > 0) 
	{
		refl += reflection(intersection, ray, depth - 1);
	
		if (myRefractionEnabled) 
		{	
			refract += refraction(intersection, ray, depth - 1, intersection.material.refractionIndex);

			/* // Hard Coded Chromatic Aberration
			resRed += refraction(intersection, ray, depth - 1, intersection.material.refractionIndex + 0.1);
			resGreen += refraction(intersection, ray, depth - 1, intersection.material.refractionIndex);
			resBlue += refraction(intersection, ray, depth - 1, intersection.material.refractionIndex - 0.1);

			res.red += resRed.red*0.8 + resGreen.red*.1 + resBlue.red*.1;
			res.green += resRed.green*0.1 + resGreen.green*.8 + resBlue.green*.1;
			res.blue += resRed.blue*0.1 + resGreen.blue*.1 + resBlue.blue*.8;*/
		}
	}
	res += refl + refract;
	return res;
}

Intersection RayTracer::findFirstHit(const Ray3D &ray, Object3D *excludedSource) {
	// Find the closest intersection between the ray and any objects in the scene.
    
	double closestLambda = DBL_MAX;
	Intersection closestIntersection;
    closestIntersection.none = true;

	for (auto it = objects.begin(); it != objects.end(); it++)
	{
        Object3D *object = *it;
        if (excludedSource == object) continue;
		Intersection intersection = object->intersect(ray);
		if (!intersection.none) 
		{
			if (intersection.lambda < closestLambda) {
				closestLambda = intersection.lambda;
				closestIntersection = intersection;
			}
		}
	}
    
    return closestIntersection;
}

ColourRGB RayTracer::phongModel(const Intersection &intersection, const Ray3D &ray) {
    ColourRGB phongColour(0, 0, 0);
    
    // Ambient component
    // Not effected by lights or shadows. Always constant.
    double ambient = intersection.material.ambient;
    ColourRGB ambientColour = ColourRGB(1, 1, 1) * ambient;
    phongColour += ambientColour.filter(intersection.colour);
    
    list<Light*>::iterator it;
    for (it=lights.begin(); it!=lights.end(); it++)
    {
        Light *light = *it;
        Point3D lightLocation = light->getLocation();
        
        if (!isInShadow(intersection, lightLocation)) 
		{
            // Note: Light have the same Ia, Id, and Is,
            // which for every colour component is equal
            // to the light's magnitude of that colour.
            // i.e. Iar = light.red, Iab = light.blue,
            //      Is.b = light.blue, etc.
            
            //Diffuse component
            Point3D s = (lightLocation - intersection.point).normalized();			// light direction
            double n_dot_s = (intersection.normal).dot(s);
            double mag_diffuse = (0 < n_dot_s)?n_dot_s:0;
            double diffuse = (intersection.material.diffuse)*mag_diffuse;
            ColourRGB diffuseColour = light->getColour() * diffuse;
            
            //Specular component
            Point3D r = -1*s + 2*intersection.normal*(s.dot(intersection.normal));	// reflection direction
            Point3D b = -1 * ray.myDirection;
            double mag_spec = (0 < r.dot(b))?r.dot(b):0;
            mag_spec = pow(mag_spec, intersection.material.shinyness);
            double specular = (intersection.material.specular)*mag_spec;
            ColourRGB specularColour = light->getColour() * specular;
            
            // Filter the ambient and diffuse components by the object's
            // colour, but the specular should just be a pure reflectance
            // of the light's colour
            phongColour +=  diffuseColour.filter(intersection.colour) +
                            specularColour;
        }
    }
    
    return phongColour;
}

bool RayTracer::isInShadow(const Intersection &intersection, const Point3D &lightLocation) {
    // Create the shadow ray
    Point3D origin = intersection.point;
    Point3D direction = (lightLocation - origin).normalized();
    Ray3D shadowRay = Ray3D(origin, direction);
    
    // Bias the ray's origin slightly away from the object
    // in order to avoid errors from numerical precision.
    shadowRay = shadowRay.bias(intersection.normal);
    
    // Walk through the objects and return that there is a shadow
    // as soon as a non light source object intsersects the shadow ray
    for (auto it = objects.begin(); it != objects.end(); it++)
    {
        Object3D *object = *it;
        if (!object->isLightSource() && object->doesIntersect(shadowRay))
            return true;
    }
    
    return false;
}

ColourRGB RayTracer::reflection(const Intersection &intersection, const Ray3D &ray, int depth) {
    ColourRGB reflectedColour(0, 0, 0);
    
    Object3D *excludedSource = intersection.canSelfReflect ? intersection.obj : NULL;
    
    // Reflection direction
    Point3D r = ray.myDirection - 2 * intersection.normal*(ray.myDirection.dot(intersection.normal));
	
    // Randomly perturb the ray in a number of samples and compute a weighted average
    // of the colour traced by these samples. The weighted average is due to the fact
    // that rays which deviated larger should have less contribution (similar to spectral in phong).
    // Weights are equal to deflected.dot(original))^shinyness.
	if (myGlossyReflEnabled) 
	{
        double totalEnergy = 0;
        for (int i = 0; i < myGlossyResolution; i++) 
		{
            Ray3D reflectionRay(intersection.point,
                                r.randomlyPerturb(intersection.normal, intersection.material.roughness));
            reflectionRay = reflectionRay.bias(intersection.normal);
            double energy = pow(r.dot(reflectionRay.myDirection), intersection.material.shinyness);
            reflectedColour += rayTraceRecursive(reflectionRay, depth, excludedSource) * energy;
            totalEnergy += energy;
        }
        if (totalEnergy > 0) 
		{
            reflectedColour = reflectedColour * (1/totalEnergy);
        }
	}
    else 
	{
        Ray3D reflectionRay(intersection.point, r);
        reflectionRay = reflectionRay.bias(intersection.normal);
        reflectedColour = rayTraceRecursive(reflectionRay, depth);
    }
    
    return reflectedColour * intersection.material.global;
}

ColourRGB RayTracer::refraction(const Intersection &intersection, const Ray3D &ray, int depth, double refractionIndex) {
	ColourRGB refractedColour(0, 0, 0);

	// If no transparency, don't even bother computing anything
    if (intersection.material.opacity >= 1.0) 
	{
        return ColourRGB(0.0, 0.0, 0.0);
    }
    
    // Assume that the transfer of medium is always from vaccuum to material or vise-versa.
    // (i.e. we do not permit refraction between two different materials)
    double n1 = intersection.insideObject ? refractionIndex : 1.0;
    double n2 = intersection.insideObject ? 1.0 : refractionIndex;
    Point3D d = ray.myDirection;
    Point3D n = intersection.normal;
    
	double inside = totalInternalReflection(intersection, ray, refractionIndex);
    if (inside < 0) // total internal reflection
	{
		return reflection(intersection, ray, depth) * (1.0 / intersection.material.global);
	}
	Point3D t = n1 / n2 * (d - n*(d.dot(n))) - n * sqrt(inside);

	// Randomly perturb the ray in a number of samples and compute a weighted average
	// of the colour traced by these samples
	if (myBlurEnabled && intersection.material.roughness > 0.0 && !intersection.insideObject) 
	{
		double totalEnergy = 0;
		for (int i = 0; i < myBlurResolution; i++) 
		{
			Ray3D refractedRay(intersection.point,
				t.randomlyPerturb(-1 * n, intersection.material.roughness));
			refractedRay = refractedRay.bias(-1 * n);
			double energy = pow(t.dot(refractedRay.myDirection), intersection.material.shinyness);
			refractedColour += rayTraceRecursive(refractedRay, depth) * energy;
			totalEnergy += energy;
		}
		if (totalEnergy > 0) 
		{
			refractedColour = refractedColour * (1 / totalEnergy);
		}
	}
	else 
	{
		Ray3D refractedRay(intersection.point, t);
		refractedRay = refractedRay.bias(-1 * n);
		refractedColour = rayTraceRecursive(refractedRay, depth);
	}
	return  refractedColour * (1 - intersection.material.opacity);
}

double RayTracer::totalInternalReflection(const Intersection &intersection, const Ray3D &ray, double refractionIndex)
{
	double n1 = intersection.insideObject ? refractionIndex : 1.0;
	double n2 = intersection.insideObject ? 1.0 : refractionIndex;
	Point3D d = ray.myDirection;
	Point3D n = intersection.normal;

	return 1 - (n1 / n2)*(n1 / n2)*(1 - (d.dot(n))*(d.dot(n)));
}

void RayTracer::renderNoiseImage(Image *output, char * name, vector<int> bounds)
{
	// Render a .ppm using Perlin Noise

	int rows = bounds[1] - bounds[0];
	int cols = bounds[3] - bounds[2];

	unsigned int seed = 227;
	PerlinNoise pn(seed);

	// Object to keep track of the progress and give us some feedback
	ProgressManager progressManager(rows * cols);
	progressManager.startTimer();

	// Itterate through all the pixels and do the ray tracing
#pragma omp parallel for schedule(dynamic)		// Multithread rendering
	for (int i = 0; i < output->myX; i++)		
	{
		for (int j = 0; j < output->myY; j++)
		{
			if (i < bounds[0] || i >= bounds[1] || j < bounds[2] || j >= bounds[3]) {
				output->setColourAtPixel(i, j, ColourRGB(0, 0, 0));
				continue;
			}
			double x = (double)i / output->myX;
			double y = (double)j / output->myY;

			//Wood-like texture
			double n = 20 * pn.noise(x, y, 0.8);
			n = n - floor(n);

			ColourRGB pixelColour;
			pixelColour.myRed = n;
			pixelColour.myGreen = n;
			pixelColour.myBlue = n;

			output->setColourAtPixel(i, j, pixelColour);
			progressManager.advance();
		}
	}
	// Output rendered image
	output->outputImage(name);
}
