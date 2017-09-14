#include "Integrator.h"
/*
//Integrator::~Integrator() { }

BasicIntegrator::BasicIntegrator(Scene scene) : myScene(scene) {}

ColourRGB BasicIntegrator::shade(Intersection &intersection, const Ray3D &ray, int depth) 
{
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

		if (myScene.myProp.myRefractionEnabled)
		{
			refract += refraction(intersection, ray, depth - 1, intersection.material.refractionIndex);

			/* // Hard Coded Chromatic Aberration
			resRed += refraction(intersection, ray, depth - 1, intersection.material.refractionIndex + 0.1);
			resGreen += refraction(intersection, ray, depth - 1, intersection.material.refractionIndex);
			resBlue += refraction(intersection, ray, depth - 1, intersection.material.refractionIndex - 0.1);

			res.red += resRed.red*0.8 + resGreen.red*.1 + resBlue.red*.1;
			res.green += resRed.green*0.1 + resGreen.green*.8 + resBlue.green*.1;
			res.blue += resRed.blue*0.1 + resGreen.blue*.1 + resBlue.blue*.8;*/
		/*}
	}
	res += refl + refract;
	return res;
}

ColourRGB BasicIntegrator::phongModel(const Intersection &intersection, const Ray3D &ray)
{
	ColourRGB phongColour(0, 0, 0);

	// Ambient component
	// Not effected by lights or shadows. Always constant.
	double ambient = intersection.material.ambient;
	ColourRGB ambientColour = ColourRGB(1, 1, 1) * ambient;
	phongColour += ambientColour.filter(intersection.colour);

	list<Light*>::iterator it;
	for (it = myScene.myLights.begin(); it != myScene.myLights.end(); it++)
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
			double mag_diffuse = (0 < n_dot_s) ? n_dot_s : 0;
			double diffuse = (intersection.material.diffuse)*mag_diffuse;
			ColourRGB diffuseColour = light->getColour() * diffuse;

			//Specular component
			Point3D r = -1 * s + 2 * intersection.normal*(s.dot(intersection.normal));	// reflection direction
			Point3D b = -1 * ray.myDirection;
			double mag_spec = (0 < r.dot(b)) ? r.dot(b) : 0;
			mag_spec = pow(mag_spec, intersection.material.shinyness);
			double specular = (intersection.material.specular)*mag_spec;
			ColourRGB specularColour = light->getColour() * specular;

			// Filter the ambient and diffuse components by the object's
			// colour, but the specular should just be a pure reflectance
			// of the light's colour
			phongColour += diffuseColour.filter(intersection.colour) + specularColour;
		}
	}

	return phongColour;
}

bool BasicIntegrator::isInShadow(const Intersection &intersection, const Point3D &lightLocation)
{
	// Create the shadow ray
	Point3D origin = intersection.point;
	Point3D direction = (lightLocation - origin).normalized();
	Ray3D shadowRay = Ray3D(origin, direction);

	// Bias the ray's origin slightly away from the object
	// in order to avoid errors from numerical precision.
	shadowRay = shadowRay.bias(intersection.normal);

	// Walk through the objects and return that there is a shadow
	// as soon as a non light source object intsersects the shadow ray
	for (auto it = myScene.myObjects.begin(); it != myScene.myObjects.end(); it++)
	{
		Object3D *object = *it;
		if (!object->isLightSource() && object->doesIntersect(shadowRay))
			return true;
	}

	return false;
}

ColourRGB BasicIntegrator::rayTraceRecursive(const Ray3D &ray, int depth, Object3D *excludedSource)
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

ColourRGB BasicIntegrator::reflection(const Intersection &intersection, const Ray3D &ray, int depth)
{
	ColourRGB reflectedColour(0, 0, 0);

	Object3D *excludedSource = intersection.canSelfReflect ? intersection.obj : NULL;

	// Reflection direction
	Point3D r = ray.myDirection - 2 * intersection.normal*(ray.myDirection.dot(intersection.normal));

	// Randomly perturb the ray in a number of samples and compute a weighted average
	// of the colour traced by these samples. The weighted average is due to the fact
	// that rays which deviated larger should have less contribution (similar to spectral in phong).
	// Weights are equal to deflected.dot(original))^shinyness.
	if (myScene.myProp.myGlossyReflEnabled)
	{
		double totalEnergy = 0;
		for (int i = 0; i < myScene.myProp.myGlossyResolution; i++)
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
			reflectedColour = reflectedColour * (1 / totalEnergy);
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

ColourRGB BasicIntegrator::refraction(const Intersection &intersection, const Ray3D &ray, int depth, double refractionIndex)
{

}

double BasicIntegrator::totalInternalReflection(const Intersection &intersection, const Ray3D &ray, double refractionIndex)
{

}
*/
