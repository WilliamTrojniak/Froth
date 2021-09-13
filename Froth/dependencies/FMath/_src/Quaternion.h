#pragma once
#include "Matrix3D.h"

namespace Froth::Math
{
	class Quaternion
	{
	public:
		float x, y, z, w;

		Quaternion() = default;
		Quaternion(float a, float b, float c, float s);
		Quaternion(const Vector3D& v, float s);

		const Vector3D& getVector() const;

		Matrix3D getRotationMatrix() const;
		void setRotationMatrix(const Matrix3D& M);
		
		static Vector3D transformVector(const Vector3D& v, const Quaternion& q);
	};

	Quaternion operator * (const Quaternion& q1, const Quaternion& q2);
}