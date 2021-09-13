#pragma once
#include "Point3D.h"
#include "Vector4D.h"

namespace Froth::Math
{
	class Plane
	{
	public:
		float x, y, z, w;

		Plane() = default;

		Plane(float nx, float ny, float nz, float d);

		Plane(const Vector3D& n, float d);

		const Vector3D& getNormal() const;

		static float dot(const Plane& f, const Vector3D& v);

		static float dot(const Plane& f, const Vector4D& v);

		static float dot(const Plane& f, const Point3D& p);
	};

}