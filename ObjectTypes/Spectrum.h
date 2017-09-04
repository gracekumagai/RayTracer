#ifndef Spectrum_h
#define Spectrum_h

#include <stdio.h>
#include "../GeometricTypes/Point3D.h"
#include "../Utilities/Helper.h"

static const int sampledLambdaStart = 400;
static const int sampledLambdaEnd = 700;
static const int nSpectralSamples = 30;

static const int nCIESamples = 471;
extern const float CIE_X[nCIESamples];
extern const float CIE_Y[nCIESamples];
extern const float CIE_Z[nCIESamples];
extern const float CIE_lambda[nCIESamples];

extern bool SpectrumSamplesSorted(const double *lambda, const double *vals, int n);
extern void SortSpectrumSamples(double *lambda, double *vals, int n);
extern double AverageSpectrumSamples(const double *lambda, const double *vals, int n, double lambdaStart, double lambdaEnd);

template <int nSamples> class CoefficientSpectrum {
public:

	CoefficientSpectrum(double v = 0.0f)
	{
		for (int i = 0; i < nSamples; ++i)
		{
			c[i] = v;
		}
	}

	CoefficientSpectrum& operator+=(const CoefficientSpectrum &s2)
	{
		for (int i = 0; i < nSamples; ++i)
		{
			c[i] += s2.c[i];
		}
		return *this;
	}

	CoefficientSpectrum operator+(const CoefficientSpectrum &s2) const
	{
		CoefficientSpectrum ret = *this;
		for (int i = 0; i < nSamples; ++i)
			ret.c[i] += s2.c[i];
		return ret;
	}

	CoefficientSpectrum& operator-=(const CoefficientSpectrum &s2)
	{
		for (int i = 0; i < nSamples; ++i)
		{
			c[i] -= s2.c[i];
		}
		return *this;
	}

	CoefficientSpectrum operator-(const CoefficientSpectrum &s2) const
	{
		CoefficientSpectrum ret = *this;
		for (int i = 0; i < nSamples; ++i)
			ret.c[i] -= s2.c[i];
		return ret;
	}

	CoefficientSpectrum& operator*=(const CoefficientSpectrum &s2)
	{
		for (int i = 0; i < nSamples; ++i)
		{
			c[i] *= s2.c[i];
		}
		return *this;
	}

	CoefficientSpectrum operator*(const CoefficientSpectrum &s2) const
	{
		CoefficientSpectrum ret = *this;
		for (int i = 0; i < nSamples; ++i)
			ret.c[i] *= s2.c[i];
		return ret;
	}

	CoefficientSpectrum& operator/=(const CoefficientSpectrum &s2)
	{
		for (int i = 0; i < nSamples; ++i)
		{
			c[i] /= s2.c[i];
		}
		return *this;
	}

	CoefficientSpectrum operator/(const CoefficientSpectrum &s2) const
	{
		CoefficientSpectrum ret = *this;
		for (int i = 0; i < nSamples; ++i)
			ret.c[i] /= s2.c[i];
		return ret;
	}

	bool operator==(const CoefficientSpectrum &lh, const CoefficientSpectrum &rh) const
	{
		for (int i = 0; i < nSamples; ++i)
		{
			if (lh.c[i] != rh.c[i])
				return false;
		}
		return true;
	}

	bool operator!=(const CoefficientSpectrum &sp) const
	{
		return !(*this == sp)
	}

	bool isBlack() const
	{
		for (int i = 0; i < nSamples; ++i)
		{
			if (c[i] != 0.0)
				return false;
			return true;
		}
	}

	friend CoefficientSpectrum Sqrt(const CoefficientSpectrum &s)
	{
		CoefficientSpectrum ret;
		for (int i = 0; i < nSamples; ++i)
		{
			ret.c[i] = sqrt(s.c[i]);
		}
		return ret;
	}

	template <int n>
	friend CoefficientSpectrum Pow(const CoefficientSpectrum &s, double e);

	friend CoefficientSpectrum Exp(const CoefficientSpectrum &s)
	{
		CoefficientSpectrum ret;
		for (int i = 0; i < nSamples; ++i)
		{
			ret.c[i] = exp(s.c[i]);
		}
		return ret;
	}

	CoefficientSpectrum Clamp(double low = 0.0, double high = Infinity) const
	{
		CoefficientSpectrum ret;
		for (int i = 0; i < nSamples; ++i)
		{
			ret.c[i] = Utility::Clamp(c[i], low, high);
		}
		return ret;
	}

	bool HasNaNs() const
	{
		for (int i = 0; i < nSamples; ++i)
		{
			if (isnan(c[i]))
				return true;
			return false;
		}
	}
protected:
	double c[nSamples];
};

class SampledSpectrum : public CoefficientSpectrum<nSpectralSamples>
{
public:
	SampledSpectrum(double v = 0.0)
	{
		for (int i = 0; i < nSpectralSamples; ++i)
		{
			c[i] = v;
		}
	}
	
	static SampledSpectrum FromSampled(const double *lambda, const double *v, int n)
	{
		// Takes array of SPD sample values v at given wavelengths lambda to define
		// piecewise linear function representing the SPD
		
		if (!SpectrumSamplesSorted(lambda, v, n))
		{
			vector<double> slambda(&lambda[0], &lambda[n]);
			vector<double> sv(&v[0], &v[n]);
			SortSpectrumSamples(&slambda[0], &sv[0], n);
			return FromSampled(&slambda[0], &sv[0], n);
		}

		SampledSpectrum r;
		for (int i = 0; i < nSpectralSamples; ++i)
		{
			double lambda0 = Lerp(double(i) / double(nSpectralSamples),
				sampledLambdaStart, sampledLambdaEnd);
			double lambda1 = Lerp(double(i + 1) / double(nSpectralSamples),
				sampledLambdaStart, sampledLambdaEnd);
			r.c[i] = AverageSpectrumSamples(lambda, v, n, lambda0, lambda1);
		}
		return r;
	}

	static void Init()
	{
		// Compute XYZ matching functions for SampledSpectrum
		for (int i = 0; i < nSpectralSamples; ++i)
		{
			double wl0 = Lerp(double(i) / double(nSpectralSamples),
				sampledLambdaStart, sampledLambdaEnd);
			double wl1 = Lerp(double(i + 1) / double(nSpectralSamples),
				sampledLambdaStart, sampledLambdaEnd);
			X.c[i] = AverageSpectrumSamples(CIE_lambda, CIE_X, nCIESamples,
				wl0, wl1);
			Y.c[i] = AverageSpectrumSamples(CIE_lambda, CIE_Y, nCIESamples,
				wl0, wl1);
			Z.c[i] = AverageSpectrumSamples(CIE_lambda, CIE_Z, nCIESamples,
				wl0, wl1);
			yint += Y.c[i];
		}
		// Compute RGB to spectrum functions for SampledSpectrum
	}

	void ToXYZ(double xyz[3]) const
	{
		xyz[0] = xyz[1] = xyz[2] = 0.0;
		for (int i = 0; i < nSpectralSamples; ++i)
		{
			xyz[0] += X.c[i] * c[i];
			xyz[1] += Y.c[i] * c[i];
			xyz[2] += Z.c[i] * c[i];
		}
		xyz[0] /= yint;
		xyz[1] /= yint;
		xyz[2] /= yint;
	}

	double ToY() const
	{
		float yy = 0.0;
		for (int i = 0; i < nSpectralSamples; ++i)
		{
			yy += Y.c[i] * c[i];
		}
		return yy / yint;
	}

private:
	static SampledSpectrum X, Y, Z;
	static float yint;
}


template <Spectrum>
inline Spectrum Lerp(float t, const Spectrum &s1, const Spectrum &s2)
{
	return (1.0 - t) * s1 + t * s2;
}

/*
// Spectrum Inline Functions
template <int nSpectrumSamples>
inline CoefficientSpectrum<nSpectrumSamples> Pow(const CoefficientSpectrum<nSpectrumSamples> &s, Float e) 
{
	CoefficientSpectrum<nSpectrumSamples> ret;
	for (int i = 0; i < nSpectrumSamples; ++i) 
		ret.c[i] = pow(s.c[i], e);
	return ret;
}*/
#endif
