#pragma once

namespace Froth::Math
{
	class Vector3D
	{
	public:
		float x, y, z;

		Vector3D() = default;

		explicit Vector3D(float n);

		Vector3D(float a, float b, float c);
	

		float operator [](size_t i);

		const float operator [](size_t i) const;

		Vector3D& operator *= (float s);

		Vector3D& operator /= (float s);

		Vector3D& operator += (const Vector3D& v);

		Vector3D& operator -= (const Vector3D& v);

		inline float getMagnitude() const;

		static Vector3D Vector3D::normalize(const Vector3D& v);

		static float dot(const Vector3D& a, const Vector3D& b);

		static Vector3D cross(const Vector3D& a, const Vector3D& b);

		static Vector3D project(const Vector3D& a, const Vector3D& b);

		static Vector3D reject(const Vector3D& a, const Vector3D& b);
	};

	Vector3D operator * (const Vector3D& v, float s);

	Vector3D operator / (const Vector3D& v, float s);

	Vector3D operator - (const Vector3D& v);
	
	Vector3D operator + (const Vector3D& a, const Vector3D& b);
	
	Vector3D operator - (const Vector3D& a, const Vector3D& b);


}

