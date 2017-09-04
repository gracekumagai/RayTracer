/*#include "Spectrum.h"
#include <algorithm>

using namespace std;

bool SpectrumSamplesSorted(const double *lambda, const double *vals, int n)
{
	for (int i = 0; i < n - 1; ++i)
	{
		if (lambda[i] > lambda[i + 1])
			return false;
		return true;
	}
}

void SortSpectrumSamples(double *lambda, double *vals, int n)
{
	vector<pair<double, double>> sortedVector;
	sortedVector.reserve(n);

	for (int i = 0; i < n; ++i)
	{
		sortedVector.push_back(make_pair(lambda[i], vals[i]));
	}
	sort(sortedVector.begin(), sortedVector.end());
	for (int i = 0; i < n; ++i)
	{
		lambda[i] = sortedVector[i].first;
		vals[i] = sortedVector[i].second;
	}
}

double AverageSpectrumSamples(const float *lambda, const double *vals, int n, double lambdaStart, double lambdaEnd)
{
	//Check edge cases
	if (lambdaEnd <= lambda[0]) return vals[0];
	if (lambdaStart >= lambda[n - 1]) return vals[n - 1];
	if (n == 1) return vals[0];

	double sum = 0.0;

	//add contributions from before and after segments
	if (lambdaStart < lambda[0])
	{
		sum += vals[0] * (lambda[0] - lambdaStart);
	}
	if (lambdaEnd > lambda[n-1])
	{
		sum += vals[n - 1] * (lambdaEnd - lambda[n - 1]);
	}

	int i = 0;
	while (lambdaStart > lambda[i + 1])
	{
		++i;
	}

#define INTERP(w, i) \
	Lerp(((w) - lambda[i]) / (lambda[(i)+1] - lambda[i]), \
	vals[i], vals[(i)+1])
#define SEG_AVG(wl0, wl1, i) (0.5f * (INTERP(wl0, i) + INTERP(wl1, i)))
	for (; i + 1 < n && lambdaEnd >= lambda[i]; ++i) {
		double segStart = max(lambdaStart, lambda[i]);
		double segEnd = min(lambdaEnd, lambda[i + 1]);
		sum += SEG_AVG(segStart, segEnd, i) * (segEnd - segStart);
	}
#undef INTERP
#undef SEG_AVG



}
*/