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
    ColourRGB rayTrace(Ray3D ray, double pixelSize, Point3D targetPixel);
    
    // Recursive version of rayTrace. rayTrace is basically the public interface
    // that delegates to this function with the correct parameters
    ColourRGB rayTraceRecursive(const Ray3D &ray, int depth, Object3D *excludedSource = NULL);
    
    // Determines the colour for the input ray
    ColourRGB shade(Intersection &intersection, const Ray3D &ray, int depth);
    
    // Finds the closest intersection between the ray and any objects in the scene.
    Intersection findFirstHit(const Ray3D &ray, Object3D *excludedSource);
    
    ColourRGB phongModel(const Intersection &intersection, const Ray3D &ray);
    
    bool isInShadow(const Intersection &intersection, const Point3D &lightLocation);
    
    ColourRGB reflection(const Intersection &intersection, const Ray3D &ray, int depth);
    
    ColourRGB refraction(const Intersection &intersection, const Ray3D &ray, int depth, double refractionIndex);
    
	double totalInternalReflection(const Intersection &intersection, const Ray3D &ray, double refractionIndex);

	ColourRGB applyFog(ColourRGB colour, double distance);

public:
    bool myAntialiasingEnabled = false;
    bool myGlossyReflEnabled = false;
    bool myRefractionEnabled = false;
	bool myBlurEnabled = false;
    int myMaxDepth = 3;
    int mySuperSamplingResolution = 5;
    int myGlossyResolution = 8;
	int myBlurResolution = 8;
    Skybox *mySkyBox = NULL;

	//Depth of field
	bool myDOFEnabled = false;
	double myFocalLength = 4.0;
	double myApeture = 0.0078;
	double myB = 0.5;
	int myDOFResolution = 500;
    
    void renderImage(View camera, list<Object3D*> objects, list<Light*> lights,
                     Image *output, char * name, vector<int> bounds);

	void renderNoiseImage(Image *output, char * name, vector<int> bounds);
};

#endif
