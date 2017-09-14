#ifndef Integrator_h
#define Integrator_h

#include<stdio.h>
#include<stdlib.h>
#include <list>
#include<math.h>
#include<string.h>
#include "Lights/Light.h"
#include "ObjectTypes/Object3D.h"
#include "ObjectTypes/Intersection.h"
#include "ObjectTypes/Ray3D.h"
#include "Scene.h"
/*
class Integrator
{
public:
	//virtual ~Integrator();
	//virtual void Render() = 0;
};

class BasicIntegrator : public Integrator
{
	Scene myScene;

	BasicIntegrator(Scene scene);

	ColourRGB shade(Intersection &intersection, const Ray3D &ray, int depth);

	ColourRGB phongModel(const Intersection &intersection, const Ray3D &ray);

	bool isInShadow(const Intersection &intersection, const Point3D &lightLocation);

	ColourRGB rayTraceRecursive(const Ray3D &ray, int depth, Object3D *excludedSource);

	ColourRGB reflection(const Intersection &intersection, const Ray3D &ray, int depth);

	ColourRGB refraction(const Intersection &intersection, const Ray3D &ray, int depth, double refractionIndex);

	double totalInternalReflection(const Intersection &intersection, const Ray3D &ray, double refractionIndex);
};
*/
#endif
