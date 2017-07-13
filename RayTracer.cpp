#include <list>
#include <time.h>
#include <float.h>
#include "RayTracer.h"
#include "Utilities/ProgressManager.h"

void RayTracer::renderImage(View camera, list<Object3D*> objects, list<Light*> lights,
                            Image *output, char * name, vector<int> bounds) {
    // Store local copies of these that way we don't have to keep passing them around
    // between the functions that do the actual ray tracing work
    this->objects = objects;
    this->lights = lights;
    
    int rows = bounds[1] - bounds[0];
    int cols = bounds[3] - bounds[2];
    // Object to keep track of the progress and give us some feedback
    ProgressManager progressManager(rows * cols);
    progressManager.startTimer();
    
    // Itterate through all the pixels and do the ray tracing
#pragma omp parallel for schedule(dynamic)   // Multithread rendering
    for (int i = 0; i < output->sx; i++)		// For each pixel in the image
    {
        for (int j = 0; j < output->sy; j++)
        {
            if (i < bounds[0] || i >= bounds[1] || j < bounds[2] || j >= bounds[3]) {
                output->setColourAtPixel(i, j, ColourRGB(0, 0, 0));
                continue;
            }
            
            ///////////////////////////////////////////////////////////////////
            // TO DO - complete the code that should be in this loop to do the
            //         raytracing!
            ///////////////////////////////////////////////////////////////////
            
            // Find the current ray
            Point3D origin(0, 0, 0, false);
            double xIncrement = ((i+0.5)/output->sx) * camera.wsize;
            double yIncrement = ((j+0.5)/output->sy) * camera.wsize;
            Point3D targetPixel(camera.wsize/2 - xIncrement,
                                camera.wsize/2 - yIncrement,
                                camera.f, false);
            Point3D direction = targetPixel - origin;
            Ray3D ray(camera.cameraToWorld * origin,
                      camera.cameraToWorld * direction);
            
            // Trace the pixel and store it in the image
            ColourRGB pixelColour = rayTrace(ray, camera.wsize / output->sx);
            if (pixelColour.outOfBounds()) {
                pixelColour.normalize();
            }
            output->setColourAtPixel(i, j, pixelColour);
            
            progressManager.advance();
        }
    }
    
    // Output rendered image
    output->outputImage(name);
}

ColourRGB RayTracer::rayTrace(Ray3D ray, double pixelSize) {
    ColourRGB pixelColour(0, 0, 0);
    
    if (antialiasingEnabled) {
        double subPixelSize = pixelSize / superSamplingResolution;
        for (int i = 0; i < superSamplingResolution; i++) {
            for (int j = 0; j < superSamplingResolution; j++) {
                // Compute a random x,y offset within the subPixelSize
                Point3D offset(drand48()*subPixelSize + i*subPixelSize - 0.5*pixelSize,
                               drand48()*subPixelSize + j*subPixelSize + 0.5*pixelSize,
                               0, true);
                
                // Move the ray by the random offset
                Ray3D subRay = ray.offset(offset);
                
                // Cast the ray. We divide by the number of super-samples we have
                // in order to compute the average colour over all rays for this pixel
                pixelColour += rayTraceRecursive(subRay, maxDepth) *
                        (1.0 / (superSamplingResolution*superSamplingResolution));
            }
        }
    }
    
    else {
        ray = ray.normalized();
        pixelColour = rayTraceRecursive(ray, maxDepth);
    }
    
    return pixelColour;
}

ColourRGB RayTracer::rayTraceRecursive(const Ray3D &ray, int depth, Object3D *excludedSource) {
    ///////////////////////////////////////////////////////
    // TO DO: Complete this function. Refer to the notes
    // if you are unsure what to do here.
    ///////////////////////////////////////////////////////
    
    Intersection firstHit = findFirstHit(ray, excludedSource);
    if (firstHit.none) {
        if (skybox != NULL) {
            return skybox->colourInDirection(ray.direction);
        }
        return ColourRGB(0, 0, 0);
    }
    else if (firstHit.isLight) {    // Lights just emmit their colour
        return firstHit.colour;
    }
    
    return shade(firstHit, ray, depth);
}

ColourRGB RayTracer::shade(const Intersection &intersection, const Ray3D &ray, int depth) {
    ColourRGB res(0.0, 0.0, 0.0);
    
    //////////////////////////////////////////////////////////////
    // TO DO: Implement this function. Refer to the notes for
    // details about the shading model.
    //////////////////////////////////////////////////////////////
    
    res += phongModel(intersection, ray);
    
    if (depth > 0) {
        res += reflection(intersection, ray, depth - 1);
        if (refractionEnabled) {
            res += refraction(intersection, ray, depth - 1);
        }
    }
    
    return res;
}

Intersection RayTracer::findFirstHit(const Ray3D &ray, Object3D *excludedSource) {
	// Find the closest intersection between the ray and any objects in the scene.
	// It returns:
	//   - The lambda at the intersection (or < 0 if no intersection)
	//   - The pointer to the object at the intersection (so we can evaluate the colour in the shading function)
	//   - The location of the intersection point (in p)
	//   - The normal at the intersection point (in n)
	//
	// Os is the 'source' object for the ray we are processing, can be NULL, and is used to ensure we don't 
	// return a self-intersection due to numerical errors for recursive raytrace calls.
	//

	/////////////////////////////////////////////////////////////
	// TO DO: Implement this function. See the notes for
	// reference of what to do in here
	/////////////////////////////////////////////////////////////
    
	double closestLambda = DBL_MAX;
	Intersection closestIntersection;
    closestIntersection.none = true;

	for (auto it = objects.begin(); it != objects.end(); it++)
	{
        Object3D *object = *it;
        if (excludedSource == object) continue;
		Intersection intersection = object->intersect(ray);
		if (!intersection.none) {
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
        
        if (!isInShadow(intersection, lightLocation)) {
            // Note: Light have the same Ia, Id, and Is,
            // which for every colour component is equal
            // to the light's magnitude of that colour.
            // i.e. Iar = light.red, Iab = light.blue,
            //      Is.b = light.blue, etc.
            
            //Diffuse component
            Point3D s = (lightLocation - intersection.point).normalized();    // light direction
            double n_dot_s = (intersection.normal).dot(s);
            double mag_diffuse = (0 < n_dot_s)?n_dot_s:0;
            double diffuse = (intersection.material.diffuse)*mag_diffuse;
            ColourRGB diffuseColour = light->getColour() * diffuse;
            
            //Specular component
            Point3D r = -1*s + 2*intersection.normal*(s.dot(intersection.normal));    // reflection direction
            Point3D b = -1 * ray.direction;
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
    // We can't simply rely on the source object here, because
    // it's perfectly legal for an object to be casting itself
    // partly in shadow (e.g. the backside of an object)
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
    Point3D r = ray.direction - 2 * intersection.normal*(ray.direction.dot(intersection.normal));
	
    // Randomly perturb the ray in a number of samples and compute a weighted average
    // of the colour traced by these samples. The weighted average is due to the fact
    // that rays which deviated larger should have less contribution (similar to spectral in phong).
    // Weights are equal to deflected.dot(original))^shinyness.
	if (glossyreflEnabled) {
        double totalEnergy = 0;
        for (int i = 0; i < glossyResolution; i++) {
            Ray3D reflectionRay(intersection.point,
                                r.randomlyPerturb(intersection.normal, intersection.material.roughness));
            reflectionRay = reflectionRay.bias(intersection.normal);
            double energy = pow(r.dot(reflectionRay.direction), intersection.material.shinyness);
            reflectedColour += rayTraceRecursive(reflectionRay, depth, excludedSource) * energy;
            totalEnergy += energy;
        }
        if (totalEnergy > 0) {
            reflectedColour = reflectedColour * (1/totalEnergy);
        }
	}
    else {
        Ray3D reflectionRay(intersection.point, r);
        reflectionRay = reflectionRay.bias(intersection.normal);
        reflectedColour = rayTraceRecursive(reflectionRay, depth);
    }
    
    return reflectedColour * intersection.material.global;
}

ColourRGB RayTracer::refraction(const Intersection &intersection, const Ray3D &ray, int depth) {
    // If no transparency, don't even bother computing anything
    if (intersection.material.opacity >= 1.0) {
        return ColourRGB(0.0, 0.0, 0.0);
    }
    
    // Assume that the transfer of medium is always from vaccuum to material or vise-versa.
    // (i.e. we do not permit refraction between two different materials)
    double n1 = intersection.insideObject ? intersection.material.refractionIndex : 1.0;
    double n2 = intersection.insideObject ? 1.0 : intersection.material.refractionIndex;
    Point3D d = ray.direction;
    Point3D n = intersection.normal;
    
    double inside = 1 - (n1/n2)*(n1/n2)*(1 - (d.dot(n))*(d.dot(n)));
    if (inside < 0) {   // total internal reflection
        return ColourRGB(0, 0, 0);
    }
    Point3D t = n1/n2 * (d - n*(d.dot(n))) - n * sqrt(inside);
    
    Ray3D refractedRay(intersection.point, t);
    refractedRay = refractedRay.bias(-1*n);
    return rayTraceRecursive(refractedRay, depth) * (1-intersection.material.opacity);
}
