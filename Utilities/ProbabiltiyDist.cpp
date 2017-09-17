#include "ProbabilityDist.h"
#include <math.h>
#include <algorithm>

CustumWeibull::CustumWeibull(double l, double k, double n, double scale, double shift)
{
	myLambda = l;
	myK = k;
	myShift = shift;
	myScale = scale;

	generateWeights(n);
}

CustumWeibull::CustumWeibull(double l, double k, double n, double max, double min, double scale, double shift)
{
	myLambda = l;
	myK = k;
	myShift = shift;
	myScale = scale;

	myMax = max;
	myMin = min;

	generateWeights(n);
}

double CustumWeibull::pdf(double x)
{
	if (x <= 0.0)
	{
		return 0;
	}
	return (myK / myLambda) * pow((x * myScale) / myLambda, myK - 1) * exp(-pow((x * myScale) / myLambda, myK)) + myShift;
}

void CustumWeibull::normalizeWeights(double n)
{
	vector<double>::iterator min = min_element(myWeights.begin(), myWeights.end());
	vector<double>::iterator max = max_element(myWeights.begin(), myWeights.end());

	for (int i = 0; i < n; i++)
	{
		myWeights[i] = (myWeights[i] - *min) / (*max - *min);
	}

}

void CustumWeibull::generateWeights(double n)
{
	myWeights.reserve(n);

	for (int i = 0; i < n; i++)
	{
		double x = i / n;
		myWeights.push_back(pdf(x));
	}

	normalizeWeights(n);
}

vector<double> CustumWeibull::getWeights()
{
	return myWeights;
}

double CustumWeibull::getWeight(int index)
{
	return myWeights[index];
}
