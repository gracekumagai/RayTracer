#ifndef AnimatedTransform_h
#define AnimatedTransform_h

#include "Point3D.h"
#include "Transform3D.h"
#include "Quaternion.h"

class AnimatedTransform
{
	// Implements keyframe animation through transform interpolation
public:
	AnimatedTransform(const Transform3D transformInit, double timeInit,
				   	  const Transform3D transformEnd, double timeEnd);

	void decompose(const Transform3D M, Vector *T, Quaternion *R, Transform3D *S);

	void interpolate(double time, Transform3D *t) const;

private:
	const double myInitTime, myEndTime;
	const Transform3D myInit, myEnd;
	const bool myIsAnimated;

	// Transformation Decomp M = TRS
	Vector T[2];				// Translation
	Quaternion R[2];			// Rotation
	Transform3D S[2];			// Scale
};

#endif
