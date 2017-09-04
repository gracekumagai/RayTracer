#pragma once
#ifndef Fractals_h
#define Fractals_h

#include "Image.h"

using namespace std;

class Fractals {
public:
	static void renderFractalImage(Image *output, char * name, vector<int> bounds);
};

#endif /* Fractals_h */
