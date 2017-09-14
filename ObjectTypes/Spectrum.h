//#ifndef Spectrum_h
//#define Spectrum_h

#include <stdio.h>
#include "../GeometricTypes/Point3D.h"
#include "../Utilities/Math.h"
/*
namespace Spectrum
{
	enum SpectrumType { SPECTRUM_REFLECTANCE, SPECTRUM_ILLUMINANT };

	static const int sampledLambdaStart = 400;
	static const int sampledLambdaEnd = 700;
	static const int nSpectralSamples = 30;

	// Data
	static const int nCIESamples = 471;
	extern const double CIE_X[nCIESamples];
	extern const double CIE_Y[nCIESamples];
	extern const double CIE_Z[nCIESamples];
	extern const double CIE_lambda[nCIESamples];
	static const double CIE_Y_integral = 106.856895;
	static const int nRGB2SpectSamples = 32;
	extern const double RGB2SpectLambda[nRGB2SpectSamples];
	extern const double RGBRefl2SpectWhite[nRGB2SpectSamples];
	extern const double RGBRefl2SpectCyan[nRGB2SpectSamples];
	extern const double RGBRefl2SpectMagenta[nRGB2SpectSamples];
	extern const double RGBRefl2SpectYellow[nRGB2SpectSamples];
	extern const double RGBRefl2SpectRed[nRGB2SpectSamples];
	extern const double RGBRefl2SpectGreen[nRGB2SpectSamples];
	extern const double RGBRefl2SpectBlue[nRGB2SpectSamples];
	extern const double RGBIllum2SpectWhite[nRGB2SpectSamples];
	extern const double RGBIllum2SpectCyan[nRGB2SpectSamples];
	extern const double RGBIllum2SpectMagenta[nRGB2SpectSamples];
	extern const double RGBIllum2SpectYellow[nRGB2SpectSamples];
	extern const double RGBIllum2SpectRed[nRGB2SpectSamples];
	extern const double RGBIllum2SpectGreen[nRGB2SpectSamples];
	extern const double RGBIllum2SpectBlue[nRGB2SpectSamples];

	inline void XYZToRGB(const double xyz[3], double rgb[3]);
	inline void RGBToXYZ(const double rgb[3], double xyz[3]);

	template <int nSamples>
	class CoefficientSpectrum;

	class SampledSpectrum;
}

template <int nSamples>
class Spectrum::CoefficientSpectrum
{
public:
	CoefficientSpectrum(double v = 0.0);
	
	CoefficientSpectrum &operator+=(const CoefficientSpectrum &s2);
	CoefficientSpectrum operator+(const CoefficientSpectrum &s2) const; 
	
	CoefficientSpectrum &operator*=(const CoefficientSpectrum &s2);
	CoefficientSpectrum operator*(const CoefficientSpectrum &s2) const;
	
	CoefficientSpectrum &operator*=(double a);
	CoefficientSpectrum operator*(double a) const;

	friend inline CoefficientSpectrum operator*(double a, const CoefficientSpectrum &s);

	bool isBlack() const;
	bool hasNaNs() const;

	CoefficientSpectrum Sqrt(const CoefficientSpectrum &s);
	CoefficientSpectrum Clamp(double low = 0, double high = INFINITY) const;

protected:
	double myC[nSamples];
};

class Spectrum::SampledSpectrum : public CoefficientSpectrum<nSpectralSamples>
{
public:
	SampledSpectrum(double v = 0.0);
	SampledSpectrum Clamp(double low = 0, double high = INFINITY) const;

	static SampledSpectrum fromSampled(const double *lambda, const double *v, int n);

	static void Init();

	void toXYZ(double xyz[3]) const;
	void toRGB(double rgb[3]) const;
	SampledSpectrum fromRGB(const double rgb[3], SpectrumType type);

	double y() const;

private:
	// Sampled Spectrum x, y, z curves
	static SampledSpectrum myX, myY, myZ;

	// Stores the integral of the Y matching curve over the wavelength range
	static double myYIntegral;

	static SampledSpectrum rgbRefl2SpectWhite, rgbRefl2SpectCyan;
	static SampledSpectrum rgbRefl2SpectMagenta, rgbRefl2SpectYellow;
	static SampledSpectrum rgbRefl2SpectRed, rgbRefl2SpectGreen;
	static SampledSpectrum rgbRefl2SpectBlue;
	static SampledSpectrum rgbIllum2SpectWhite, rgbIllum2SpectCyan;
	static SampledSpectrum rgbIllum2SpectMagenta, rgbIllum2SpectYellow;
	static SampledSpectrum rgbIllum2SpectRed, rgbIllum2SpectGreen;
	static SampledSpectrum rgbIllum2SpectBlue;
};


#endif
*/