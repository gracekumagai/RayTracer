#ifndef RayTracer_h
#define RayTracer_h

#include<stdio.h>
#include<stdlib.h>
#include <list>
#include<math.h>
#include<string.h>
#include "Lights/Light.h"
#include "ObjectTypes/Object3D.h"
#include "ObjectTypes/Intersection.h"
#include "ObjectTypes/Ray3D.h"
#include "View.h"
#include "Skybox.h"

using namespace std;

class RayTracer {
    
    list<Object3D*> objects;
    list<Light*> lights;
    
    // Ray-Tracing function. It finds the closest intersection between
    // the ray and any scene objects, calls the shading function to
    // determine the colour at this intersection, and returns the
    // colour.
    //
    // Since Ray-Tracing is recursive, this is the entry point that simply
    // calls the recursive version with the appropriate starting parameters.
    //
    // pixelSize is used if antialiasing is enabled
    ColourRGB rayTrace(Ray3D ray, double pixelSize, Point3D targetPixel);
    
    // Recursive version of rayTrace. rayTrace is basically the public interface
    // that delegates to this function with the correct parameters
    //
    // source is needed for recursive calls to ensure that findFirstHit will
    // not simply return a self-intersection due to numerical
    // errors. For the top level call, source should be NULL. And thereafter
    // it will correspond to the object from which the recursive
    // ray originates.
    ColourRGB rayTraceRecursive(const Ray3D &ray, int depth, Object3D *excludedSource = NULL);
    
    // This function implements the shading model as described in lecture. It takes
    // - An intersection data model that contains all necessary information for shading
    // - The ray (needed to determine the reflection direction to use for the global component, as well as for
    //   the Phong specular component)
    // - The current recursion depth
    //
    // Returns:
    // - The colour for this ray
    //
    ColourRGB shade(Intersection &intersection, const Ray3D &ray, int depth);
    
    
    // Find the closest intersection between the ray and any objects in the scene.
    // It returns:
    //   - An intersection data model that contains all necessary information for shading
    //
    // source is the 'source' object for the ray we are processing, can be NULL,
    // and is used to ensure we don't
    // return a self-intersection due to numerical errors for recursive raytrace calls.
    //
    Intersection findFirstHit(const Ray3D &ray, Object3D *excludedSource);
    
    ColourRGB phongModel(const Intersection &intersection, const Ray3D &ray);
    
    bool isInShadow(const Intersection &intersection, const Point3D &lightLocation);
    
    ColourRGB reflection(const Intersection &intersection, const Ray3D &ray, int depth);
    
    ColourRGB refraction(const Intersection &intersection, const Ray3D &ray, int depth, double refractionIndex);
    
	double totalInternalReflection(const Intersection &intersection, const Ray3D &ray, double refractionIndex);

	ColourRGB applyFog(ColourRGB colour, double distance);

public:
    bool antialiasingEnabled = false;
    bool glossyreflEnabled = false;
    bool refractionEnabled = false;
	bool blurEnabled = false;
    int maxDepth = 3;
    int superSamplingResolution = 5;
    int glossyResolution = 8;
	int blurResolution = 8;
    Skybox *skybox = NULL;

	//Depth of field
	bool dofEnabled = false;
	double focalLength = 2.0;
	double apeture = 0.0078;
	double b = 0.5;
	int dofResolution = 500;

	//Fog
	bool fogEnabled = false;
	double falloff = 1.0;
    
    void renderImage(View camera, list<Object3D*> objects, list<Light*> lights,
                     Image *output, char * name, vector<int> bounds);

	void renderNoiseImage(Image *output, char * name, vector<int> bounds);
};

#endif
