#include <stdio.h>
#include <list>
#include "RayTracer.h"
#include "ObjectTypes/Fractals.h"
#include "Scene.h"
#include "Utilities/ProbabilityDist.h"
#include "ObjectTypes/Spectrum.h"

using namespace std;

// maximum recursion depth
int MAX_DEPTH;

typedef RGBSpectrum Spectrum;

int main(int argc, char *argv[])
{
    // Parses input parameters, sets up the initial blank image, 
	// and calls the functions that set up the scene and do the raytracing.
    Image *im;								// Will hold the raytraced image
    int sx;									// Size of the raytraced image
    int antialiasing;						// Flag to determine whether antialiaing is enabled or disabled
    char output_name[1024];					// Name of the output file for the raytraced .ppm image
    Point3D e;								// Camera view parameters 'e', 'g', and 'up'
    Point3D g;
    Point3D up;
    ColourRGB background(0.0, 0.0, 0.0);	// Background colour
    RayTracer rayTracer;					// The ray tracer
    
    // Parallization parameters
    vector<int> bounds;
    int firstRow, lastRow;
    int firstCol, lastCol;
    
    if (argc<7)
    {
        fprintf(stderr,"RayTracer: Can not parse input parameters - Using default values\n");
        fprintf(stderr,"USAGE: RayTracer size rec_depth antialias output_name\n");
        fprintf(stderr,"   size = Image size (both along x and y)\n");
        fprintf(stderr,"   rec_depth = Recursion depth\n");
        fprintf(stderr,"   antialias = A single digit, 0 disables antialiasing. Anything else enables antialiasing\n");
        fprintf(stderr,"   output_name = Name of the output file, e.g. MyRender.ppm\n");
        fprintf(stderr,"   parallization_params = 4 params for the bounds of pixels we are generating\n");
        
		sx = 512;
		MAX_DEPTH = 1;
		antialiasing = 0;
		bounds.push_back(0);
		bounds.push_back(sx);
		bounds.push_back(0);
		bounds.push_back(sx);
		strcpy(&output_name[0], "Results/test.pgm");
    }
	else
	{
		sx=atoi(argv[1]);
		MAX_DEPTH=atoi(argv[2]);
		if (atoi(argv[3])==0) antialiasing=0; else antialiasing=1;
		strcpy(&output_name[0],argv[4]);
    
		bounds.push_back(atoi(argv[5]));
		bounds.push_back(atoi(argv[6]));
		bounds.push_back(atoi(argv[7]));
		bounds.push_back(atoi(argv[8]));
	}
    
    fprintf(stderr,"Rendering image at %d x %d\n",sx,sx);
    fprintf(stderr,"Recursion depth = %d\n",MAX_DEPTH);
    if (!antialiasing) fprintf(stderr,"Antialising is off\n");
    else fprintf(stderr,"Antialising is on\n");
    fprintf(stderr,"Output file name: %s\n",output_name);
    
    // Allocate memory for the new image
    im = new Image(sx, sx);
	    
    e = Point3D(0.0, 0.0, -3.0, false);
    g = Point3D(0.0, 0.0, 0.0, false) - e;
    up = Point3D(0, 1, 0, true);
    View cam(e, g, up, -3, 4);

	// Define scene
	Scene scene(MAX_DEPTH, antialiasing);
	scene.buildSceneTestDisk();

    fprintf(stderr,"View parameters:\n");
    fprintf(stderr,"Width=%f, f=%f\n", cam.myWindowSize,cam.myFocalLength);
    fprintf(stderr,"Camera to world conversion matrix (make sure it makes sense!):\n");
    cam.myCameraToWorld.printTransform3D();
    fprintf(stderr,"World to camera conversion matrix\n");
    cam.myWorldToCamera.printTransform3D();
    fprintf(stderr,"\n");
    
    // Render the image with ray tracing
    rayTracer.myMaxDepth = MAX_DEPTH;
    rayTracer.myAntialiasingEnabled = antialiasing;
    rayTracer.mySuperSamplingResolution = 5;
    rayTracer.myGlossyReflEnabled = false;
    rayTracer.myRefractionEnabled = false;
	rayTracer.myBlurEnabled = false;
	rayTracer.myDOFEnabled = false;

	// If animation is enabled
	if (scene.myProp.myAnimResolution > 1)
	{
		// Render Images at KeyFrames
		CustumWeibull weibullDist(1.0, 5.0, rayTracer.myAnimResolution);

		for (int n = 0; n < rayTracer.myAnimResolution; n++)
		{
			// Update Transformations
			scene.update(double(n) / double(rayTracer.myAnimResolution - 1.0));
			//scene.update(weibullDist.getWeight(n));

			// Rename
			char num[10];
			sprintf(num, "%d", n);

			char * newName = strdup(output_name);
			newName = strcat(newName, num);
			newName = strcat(newName, ".ppm");

			rayTracer.renderImage(cam, scene, im, newName, bounds);
		}
		rayTracer.renderAnimImage(im, output_name, bounds);
	}
	else
	{
		rayTracer.renderImage(cam, scene, im, output_name, bounds);
	}

	// Clean up
	delete im;
	
	scene.cleanUpScene();
    return 0;
}
