#pragma once
#include "Vector3D.h"


namespace Froth::Math
{
	class Point3D : public Vector3D
	{
	public:

		Point3D() = default;

		Point3D(float n) : Vector3D(n) {};

		Point3D(float a, float b, float c) : Vector3D(a, b, c) {}

		explicit Point3D(const Vector3D& v) : Vector3D(v) {}
	};

	Point3D operator + (const Point3D& a, const Vector3D& b);
	Point3D operator - (const Point3D& a, const Vector3D& b);

	Vector3D operator - (const Point3D& a, const Point3D& b);
}