#ifndef Scene_h
#define Scene_h

#include<stdio.h>
#include<stdlib.h>
#include <list>
#include "Lights/Light.h"
#include "ObjectTypes/Object3D.h"
#include "Skybox.h"

using namespace std;

struct sceneProperties
{
	bool myAntialiasingEnabled = false;
	bool myGlossyReflEnabled = false;
	bool myRefractionEnabled = false;
	bool myBlurEnabled = false;
	int myMaxDepth = 3;
	int mySuperSamplingResolution = 5;
	int myGlossyResolution = 8;
	int myBlurResolution = 8;
	Skybox *mySkybox = NULL;

	//Depth of field
	bool myDOFEnabled = false;
	double myFocalLength = 4.0;
	double myApeture = 0.0078;
	double myB = 0.5;
	int myDOFResolution = 500;
};

class Scene
{
public:

	list<Object3D*> myObjects;
	list<Light*> myLights;

	sceneProperties myProp;

	Scene();
	Scene(int maxDepth, bool antialiasing);
	~Scene();

	void buildScene();
	void buildSceneDOF();
	void buildSceneGlass();
	void buildSceneStillLife();

	void cleanUpScene();
};

#endif
