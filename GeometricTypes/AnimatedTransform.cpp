#include "AnimatedTransform.h"
#include "../Utilities/Math.h"

AnimatedTransform::AnimatedTransform(const Transform3D transformInit, double timeInit,
	const Transform3D transformEnd, double timeEnd) : myInitTime(timeInit), myEndTime(timeEnd), 
	myInit(transformInit), myEnd(transformEnd), myIsAnimated(myInit != myEnd)
{ 
	decompose(transformInit, &T[0], &R[0], &S[0]);
	decompose(transformEnd, &T[1], &R[1], &S[1]);

	if (R[0].dot(R[1]) < 0.0)	// Get shortest rotation
	{
		R[1] = R[1] * (-1.0);
	}
}

void AnimatedTransform::decompose(const Transform3D m, Vector *T, Quaternion *R, Transform3D *S)
{
	// Find T from m
	T->x = m.get(0, 3);
	T->y = m.get(1, 3);
	T->z = m.get(2, 3);

	// Focus on Pure Rotation Component
	Transform3D M = m;
	for (int i = 0; i < 3; ++i)
	{
		M.set(i, 3, 0.0);
		M.set(3, i, 0.0);
	}
	M.set(3, 3, 1.0);

	// Extract rot R - pure rotation component
	// M_(i + 1) = 1/2 (M_i + ((M_i)^T)^-1)
	double norm;
	int count = 0;
	Transform3D rot = M;
	do
	{
		Transform3D rotNext;
		Transform3D rotIT = (rot.transpose()).inverse();
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				rotNext.set(i, j, 0.5 * (rot.get(i, j) + rotIT.get(i, j)));
			}
		}

		norm = 0.0;
		for (int i = 0; i < 3; ++i)
		{
			double res = abs(rot.get(i, 0) - rotNext.get(i, 0)) +
						 abs(rot.get(i, 1) - rotNext.get(i, 1)) +
						 abs(rot.get(i, 2) - rotNext.get(i, 2));
			norm = Utility::max(norm, res);
		}

		rot = rotNext;
	} while (++count < 100 && norm > 0.0001);
	
	*R = Quaternion(rot);

	// Find S, where S = R^-1 * M
	*S = (rot.inverse()) * M;
}

void AnimatedTransform::interpolate(double time, Transform3D *t) const
{
	// Boundary conditions
	if (!myIsAnimated || time <= myInitTime) // Not animated or before start time
	{
		*t = myInit;
		return;
	}
	if (time >= myEndTime)					// after last transform
	{
		*t = myEnd;
		return;
	}

	double dt = (time - myInitTime) / (myEndTime - myInitTime);

	// Interp translation
	Vector translation = T[0] * (1.0 - dt) + T[1] * dt;

	// Interp rot
	Quaternion rotate = Slerp(R[0], R[1], dt);

	// Interp scale
	Transform3D scale;
	scale = scale.identity();

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			scale.set(i, j, Utility::Lerp(dt, S[0].get(i, j), S[1].get(i, j)));
		}
	}
	
	Transform3D trans = translate(translation);
	Transform3D rotationFromQ = rotate.findMatrix();

	// Compute interpolated matrix from T, R, S
	*t = trans * rotationFromQ * scale;
}
