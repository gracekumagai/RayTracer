#ifndef ProbabilityDist_h
#define ProbabilityDist_h

#include<stdio.h>
#include<stdlib.h>
#include <vector>

using namespace std;

class Probability
{
public:
	
};

class CustumWeibull : public Probability
{
public:
	double myLambda, myK, myScale, myShift;

	CustumWeibull(double l, double k, double n, double scale = 1.0, double shift = 0.0);
	CustumWeibull(double l, double k, double n, double max, double min, double scale = 1.0, double shift = 0.0);

	vector<double> getWeights();
	double getWeight(int index);

private:
	double myMax = 1.0, myMin = 0.0;

	vector<double> myWeights;

	double pdf(double x);
	void generateWeights(double n);
	void normalizeWeights(double n);
};
#endif
