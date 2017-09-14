#include "Fractals.h"
#include "../Utilities/ProgressManager.h"
#include <cmath>
#include <vector>

namespace 
{
	const int frames = 10000000;
	const int iters = 1000;
	const int skipIters = 10;

	double sensitivity = 0.0001;

	const double minX = -4.0;
	const double minY = minX;

	const double maxX = 4.0f;
	const double maxY = maxX;

	const double minA = acos(1.6 / 2.0);
	const double maxA = acos(1.3 / 2.0);

	const double minB = acos(-0.6 / 2.0);
	const double maxB = acos(1.7 / 2.0);

	const double minC = acos(-1.2 / 2.0);
	const double maxC = acos(0.5 / 2.0);

	const double minD = acos(1.6 / 2.0);
	const double maxD = acos(1.4 / 2.0);
}

void Fractals::renderFractalImage(Image *output, char * name, vector<int> bounds)
{
	int rows = bounds[1] - bounds[0];
	int cols = bounds[3] - bounds[2];

	ProgressManager progressManager(frames);
	progressManager.startTimer();

	vector<ColourRGB> image(output->myX * output->myY);

	// Set background colour
	for (int y = 0; y < output->myY; y++) {
		for (int x = 0; x < output->myX; x++) {
			image[x + y*output->myX] = ColourRGB(0.0, 0.0, 0.0);
		}
	}

#pragma omp parallel for schedule(dynamic) 
	for (int i = 0; i < frames; i++)
	{
		const double p = static_cast<double>(i) / frames;
		const double a = cos(minA + p * (maxA - minA)) * 2.0;
		const double b = cos(minB + p * (maxB - minB)) * 2.0;
		const double c = cos(minC + p * (maxC - minC)) * 2.0;
		const double d = cos(minD + p * (maxD - minD)) * 2.0;

		const ColourRGB curCol = ColourRGB::createHue(p);

		double x = 0.0, y = 0.0;

		for (int j = 0; j < iters; j++) {

			double xn = sin(a * y) + c * cos(a * x);
			double yn = sin(b * x) + d * cos(b * y);
			x = xn;
			y = yn;

			if (j < skipIters)
				continue;

			int xi = static_cast<int>((x - minX) * output->myX / (maxX - minX));
			int yi = static_cast<int>((y - minY) * output->myY / (maxY - minY));
			if (xi >= 0 && xi < output->myX &&
				yi >= 0 && yi < output->myY) {
				image[xi + yi * output->myX] += curCol;
			}
		}
		progressManager.advance();
	}

	for (int x = 0; x < output->myX; x++)
	{
		for (int y = 0; y < output->myY; y++)
		{
			ColourRGB colour = image[x + y*output->myX];
			output->setColourAtPixel(x, y, ColourRGB(1.0 - exp(-sensitivity * colour.myRed), 
				1.0 - exp(-sensitivity * colour.myGreen), 1.0 - exp(-sensitivity * colour.myBlue)));
		}
	}
	output->outputImage(name);
}