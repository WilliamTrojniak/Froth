#pragma once
#include "Matrix4D.h"
#include "Plane.h"


namespace Froth::Math
{
	class Transform4D : public Matrix4D
	{
	public:
		Transform4D() = default;

		Transform4D(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23);
	
		Transform4D(const Vector3D& a, const Vector3D& b, const Vector3D& c, const Point3D& p);

		Vector3D& operator [](size_t j);

		const Vector3D& operator [](size_t j) const;

		const Point3D& getTranslation() const;

		void setTranslation(const Point3D& p);

		static Transform4D inverse(const Transform4D& H);

		static Transform4D makeReflection(const Plane& f);
	};

	Transform4D operator *(const Transform4D& A, const Transform4D& B);

	Vector3D operator * (const Transform4D& H, const Vector3D& v);

	// For row vectors
	Vector3D operator * (const Vector3D& n, const Transform4D& H);

	Point3D operator * (const Transform4D& H, const Point3D& p);

	Plane operator * (const Plane& f, const Transform4D& H);
}	