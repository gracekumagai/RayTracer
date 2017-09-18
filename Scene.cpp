#include "Scene.h"
#include "ObjectTypes/ObjectSubclasses/Plane.h"
#include "ObjectTypes/ObjectSubclasses/Sphere.h"
#include "ObjectTypes/ObjectSubclasses/TriangleMesh.h"
#include "ObjectTypes/ObjectSubclasses/Disk.h"
#include "ObjectTypes/ObjectSubclasses/Torus.h"
#include "Lights/PointLightSource.h"
#include "Lights/AreaLightElement.h"

#define PI 3.14159265354

Scene::Scene() { }

Scene::Scene(int maxDepth, bool antialiasing)
{
	myProp.myMaxDepth = maxDepth;
	myProp.myAntialiasingEnabled = antialiasing;
}

Scene::~Scene()
{
}

void Scene::buildSceneStillLife(void)
{
	// BACKGROUND
	Object3D *obj = new Plane(Material(0.05, 0.95, 0.0, 0.2, 1, 15, 0, 0.3),
		ColourRGB(1.0, 0.25, 0.25));
	double height = 25, aspectRatio = 2560.0 / 1600, distance = 10;
	obj->scale(height*aspectRatio, height, 1);
	obj->translate(0, 10, distance);
	obj->loadTexture("Textures/BACKGROUND_OIL.ppm");
	myObjects.push_front(obj);

	//Shiny King
	vector<Material> kingMaterials(1, Material(0.0, 0.95, 0.5, 0.15, 1.0, 1.0, 32, 0.1));
	vector<ColourRGB> kingColours(1, ColourRGB(1.0, 1.0, 1.0));
	TriangleMesh *king = new TriangleMesh("OBJ/high_res_king.obj", kingMaterials, kingColours);
	king->scale(1.05, 1.05, 1.05);
	king->rotateX(-PI / 25);
	king->rotateY(PI);
	king->rotateZ(-PI / 27);
	king->translate(-0.3, -0.257, -1.65);

	myObjects.push_front(king);

	//Matte Horse Pawn
	vector<Material> horseMaterials(1, Material(0.0, 0.2, 1.0, 0.2, 1.0, 1.0, 3, 0.1));
	vector<ColourRGB> horseColours(1, ColourRGB(0.25, 0.25, 0.25));
	TriangleMesh *horse = new TriangleMesh("OBJ/high_res_horse.obj", horseMaterials, horseColours);
	horse->scale(1, 1, 1);
	horse->rotateX(PI / 25);
	horse->rotateY(PI);
	horse->translate(0.3, 0.12, -1.0);

	myObjects.push_front(horse);

	//Glass Pawn
	vector<Material> pawnMaterials(1, Material(0, 0, 0, 0.1, 0, 1.5, 96, 0));
	vector<ColourRGB> pawnColours(1, ColourRGB(1.0, 1.0, 1.0));
	TriangleMesh *pawn = new TriangleMesh("OBJ/high_res_pawn.obj", pawnMaterials, pawnColours);
	pawn->scale(.85, .85, .85);
	pawn->rotateX(-PI / 25);
	pawn->translate(-0.5, 0.04, -0.85);

	myObjects.push_front(pawn);

	//Chrome Queen
	vector<Material> queenMaterials(1, Material(0.25, 0.1, 0.718, 0.8, 1.0, 1.0, 100.8, 0.02));
	vector<ColourRGB> queenColours(1, ColourRGB(0.155, 0.12, 0.12));
	TriangleMesh *tri = new TriangleMesh("OBJ/high_res_queen.obj", queenMaterials, queenColours);
	tri->scale(1.05, 1.05, 1.05);
	tri->rotateX(-PI / 25);
	tri->translate(0.5, 0.063, -1.63);

	myObjects.push_front(tri);

	//Table
	vector<Material> tableMaterials(1, Material(0.25, 1.0, 0.64, 0.5, 1, 1, 6, 0.1));
	vector<ColourRGB> tableColours(1, ColourRGB(1.0, 1.0, 1.0));
	TriangleMesh *top = new TriangleMesh("OBJ/top.obj", tableMaterials, tableColours);
	top->scale(4., 4., 4.);
	top->rotateX(-PI / 25);
	top->translate(0.0, -0.452, -1.25);
	top->loadTextureForMesh(0, "OBJ/green.ppm");
	myObjects.push_front(top);

	//Area Light
	Point3D lightPosition(-12, 9, -16, false);
	Point3D lightDirection = (Point3D(0, -0.452, -1.25, false) - lightPosition).normalized();
	AreaLightElement::addAreaLight(2.5, 2.5, lightDirection, Point3D(0, 0, 1, true).crossUnit(lightDirection),
		lightPosition, 8, 8, ColourRGB(0.8, 0.8, 0.8), myLights);
}

void Scene::buildSceneGlass(void)
{
	//Plane
	Object3D *obj = new Plane(Material(0.0, 0.5, 0.0, 1.0, 0.5, 1.5, 100.8, 0.0),
		ColourRGB(1.0, 1.0, 1.0));
	obj->scale(20, 20, 2);
	obj->rotateZ(PI / 1.20);
	obj->rotateX(PI / 2.25);
	obj->translate(0, 0, 10);
	myObjects.push_front(obj);

	//Sphere
	obj = new Sphere(Material(0.05, 0.95, 0.35, 0.35, 1, 1, 6, 0.3),
		ColourRGB(1.0, 0.2, 0.2));
	obj->scale(2.0, 2.0, 2.0);
	obj->translate(0.0, 1.5, 7.0);
	obj->loadTexture("Textures/webtreats_stone_5.ppm");
	myObjects.push_front(obj);

	//Point Light Source
	PointLightSource *light = new PointLightSource(ColourRGB(0.95, 0.95, 0.95),       // original
		Point3D(0.0, 15.5, -5.5, false));
	myLights.push_front(light);
}

void Scene::buildScene(void)
{
	//Generic Scene

	//Plane
	Object3D *obj = new Plane(Material(0.05, 0.75, 0.05, 0.05, 0.5, 1, 2, 0.3),  // original
		ColourRGB(0.55, 0.8, 0.75));
	obj->scale(12, 12, 2);
	obj->rotateZ(PI / 1.20);
	obj->rotateX(PI / 2.25);
	obj->translate(0, -3, 10);
	obj->loadTexture("Textures/greyscale_natural_grunge2.ppm");
	myObjects.push_front(obj);

	obj = new Sphere(Material(0.05, 0.95, 0.35, 0.35, 1, 1, 6, 0.3),
		ColourRGB(1.0, 0.25, 0.25));
	obj->scale(0.75, 0.5, 1.5);
	obj->rotateY(PI / 2.0);
	obj->translate(-1.45, 1.1, 3.5);
	myObjects.push_front(obj);

	obj = new Sphere(Material(0.05, 0.95, 0.95, 0.75, 1, 1, 6, 0.3),
		ColourRGB(0.75, 0.95, 0.55));
	obj->scale(0.5, 2.0, 1.0);
	obj->rotateZ(PI / 1.5);
	obj->translate(1.75, 1.25, 5.0);
	myObjects.push_front(obj);

	//Point Light Source
	PointLightSource *light = new PointLightSource(ColourRGB(0.95, 0.95, 0.95),       // original
		Point3D(0.0, 15.5, -5.5, false));
	myLights.push_front(light);
}

void Scene::buildSceneDOF(void)
{
	//Plane
	Object3D *obj = new Plane(Material(0.05, 0.75, 0.05, 0.05, 0.5, 1, 2, 0.3),  // original
		ColourRGB(0.55, 0.8, 0.75));
	obj->scale(12, 12, 2);
	obj->rotateZ(PI / 1.20);
	obj->rotateX(PI / 2.25);
	obj->translate(0, -3, 10);
	myObjects.push_front(obj);

	//Sphere
	obj = new Sphere(Material(0.05, 0.8, 0.4, 0.35, 1, 1, 6, 0.3),
		ColourRGB(0.5, 0.25, 0.5));
	obj->scale(0.75, 0.5, 1.5);
	obj->rotateY(PI / 3.5);
	obj->translate(-0.5, 0.8, 10.0);
	myObjects.push_front(obj);

	//Sphere
	obj = new Sphere(Material(0.05, 0.95, 0.95, 0.75, 1, 1, 6, 0.3),
		ColourRGB(0.75, 0.95, 0.55));
	obj->scale(0.5, 2.0, 1.0);
	obj->rotateZ(PI / 1.5);
	obj->translate(1.75, 1.25, 5.0);
	myObjects.push_front(obj);

	//Point Light Source
	PointLightSource *light = new PointLightSource(ColourRGB(0.95, 0.95, 0.95),       // original
		Point3D(0.0, 15.5, -5.5, false));
	myLights.push_front(light);

	myProp.mySkybox = new Skybox("Skyboxes/lagoon_lf.ppm", "Skyboxes/lagoon_rt.ppm",
	                    "Skyboxes/lagoon_dn.ppm", "Skyboxes/lagoon_up.ppm",
	                    "Skyboxes/lagoon_bk.ppm", "Skyboxes/lagoon_ft.ppm");
}

void Scene::buildSceneAnimate(void)
{
	//Sphere
	Object3D *obj = new Sphere(Material(0.1, 0.95, 0.1, 0.0, 1, 1, 6, 0.3),
		ColourRGB(1.00, 0.00, 1.0));

	Transform3D t1 = Transform3D::translated(0.0, 0.0, 5.0) * Transform3D::scaled(3.0, 3.0, 3.0);
	Transform3D t2 = Transform3D::rotatedZ(PI) * t1;

	obj->transform = t1;
	obj->updateInverse();

	obj->animTransform = new AnimatedTransform(t1, 0, t2, 1.0);

	obj->loadTexture("RenderedImages/Wood.ppm");
	myObjects.push_front(obj);

	//Point Light Source
	PointLightSource *light = new PointLightSource(ColourRGB(0.95, 0.95, 0.95),       // original
		Point3D(0.0, 5.5, -5.5, false));
	myLights.push_front(light);
}

void Scene::buildSceneAnimate2(void)
{
	//Sphere
	Object3D *obj = new Sphere(Material(0.1, 0.95, 0.1, 0.0, 1, 1, 6, 0.3),
		ColourRGB(1.0, 1.0, 1.0));

	Transform3D t1 = Transform3D::translated(0.0, 0.0, 5.0) * Transform3D::scaled(3.0, 3.0, 3.0);
	//Transform3D t2 = Transform3D::translated(0.0, -2.0, 0.0) * t1;
	obj->transform = t1;
	obj->updateInverse();

	//obj->animTransform = new AnimatedTransform(t1, 0, t2, 1.0);
	myObjects.push_front(obj);

	//Point Light Source
	PointLightSource *light = new PointLightSource(ColourRGB(0.95, 0.95, 0.95),       // original
		Point3D(0.0, 5.5, -5.5, false));
	myLights.push_front(light);
}

void Scene::buildSceneTestDisk()
{
	// Disk
	Object3D *obj = new Torus(Material(0.75, 0.8, 0.4, 0.35, 1, 1, 6, 0.3),
		ColourRGB(0.5, 0.25, 0.5));
	//obj->scale(2.0, 2.0, 2.0);
	obj->rotateX(-PI/4.0);
	obj->translate(0.0, 1.0, 5.0);
	obj->loadTexture("Textures/webtreats_stone_5.ppm");
	myObjects.push_front(obj);

	/*obj = new Sphere(Material(0.75, 0.8, 0.4, 0.35, 1, 1, 6, 0.3),
		ColourRGB(0.5, 0.25, 0.5));
	obj->translate(-3.0, 0.0, 5.0);
	obj->loadTexture("Textures/webtreats_stone_5.ppm");
	myObjects.push_front(obj);*/

	//Point Light Source
	PointLightSource *light = new PointLightSource(ColourRGB(0.95, 0.95, 0.95),       // original
		Point3D(0.0, 15.5, -5.5, false));
	myLights.push_front(light);
}

void Scene::cleanUpScene(void)
{
	while (!myObjects.empty()) 
	{
		if (myObjects.front()->animTransform != NULL)
			delete myObjects.front()->animTransform;

		delete myObjects.front();
		myObjects.pop_front();
	}
	while (!myLights.empty()) 
	{
		delete myLights.front();
		myLights.pop_front();
	}

	if (myProp.mySkybox != NULL)
	{
		delete myProp.mySkybox;
	}
}

void Scene::update(double t)
{
	for (auto object : myObjects)
	{
		if (object->animTransform != NULL)
		{
			Transform3D T = object->transform;
			object->animTransform->interpolate(t, &T);

			object->transform = T;
			object->updateInverse();
		}
	}
}
