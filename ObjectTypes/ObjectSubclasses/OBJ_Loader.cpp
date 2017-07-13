#include "OBJ_Loader.h"

namespace objl{

	namespace math{

		// Vector3 Cross Product
		Vector3 CrossV3(const Vector3 a, const Vector3 b)
		{
			return Vector3(a.Y * b.Z - a.Z * b.Y,
				a.Z * b.X - a.X * b.Z,
				a.X * b.Y - a.Y * b.X);
		}

		// Vector3 Magnitude Calculation
		float MagnitudeV3(const Vector3 in)
		{
			return (sqrtf(powf(in.X, 2) + powf(in.Y, 2) + powf(in.Z, 2)));
		}

		// Vector3 DotProduct
		float DotV3(const Vector3 a, const Vector3 b)
		{
			return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
		}

		// Angle between 2 Vector3 Objects
		float AngleBetweenV3(const Vector3 a, const Vector3 b)
		{
			float angle = DotV3(a, b);
			angle /= (MagnitudeV3(a) * MagnitudeV3(b));
			return angle = acosf(angle);
		}
	}
	namespace algorithm
	{
		// Vector3 Multiplication Opertor Overload
		Vector3 operator*(const float& left, const Vector3& right)
		{
			return Vector3(right.X * left, right.Y * left, right.Z * left);
		}

		// Check to see if a Vector3 Point is within a 3 Vector3 Triangle
		bool inTriangle(Vector3 point, Vector3 tri1, Vector3 tri2, Vector3 tri3)
		{
			// Starting vars
			Vector3 u = tri2 - tri1;
			Vector3 v = tri3 - tri1;
			Vector3 w = point - tri1;
			Vector3 n = math::CrossV3(u, v);

			float y = (math::DotV3(math::CrossV3(u, w), n) / math::DotV3(n, n));
			float b = (math::DotV3(math::CrossV3(u, w), n) / math::DotV3(n, n));
			float a = 1 - y - b;

			// Projected point
			Vector3  p = (a * tri1) + (b * tri2) + (y * tri3);

			if (a >= 0 && a <= 1
				&& b >= 0 && b <= 1
				&& y >= 0 && y <= 1)
			{
				return true;
			}
			else
				return false;
		}
	}
}

