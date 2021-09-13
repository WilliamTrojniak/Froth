#pragma once
#include "Point3D.h"

namespace Froth::Math
{
	class Vector4D
	{
	public:
		float x, y, z, w;

		Vector4D() = default;

		Vector4D(float a, float b, float c, float d);

		Vector4D(const Vector3D& v);

		Vector4D(const Point3D& p);

		float operator [](size_t i);

		const float operator [](size_t i) const;

		Vector4D& operator *= (float s);

		Vector4D& operator /= (float s);

		Vector4D& operator += (const Vector4D& v);

		Vector4D& operator -= (const Vector4D& v);

		inline float getMagnitude() const;

		static float dot(const Vector4D& a, const Vector4D& b);

	};

	Vector4D operator * (const Vector4D& v, float s);

	Vector4D operator / (const Vector4D& v, float s);

	Vector4D operator - (const Vector4D& v);

	Vector4D operator + (const Vector4D& a, const Vector4D& b);

	Vector4D operator - (const Vector4D& a, const Vector4D& b);


}

