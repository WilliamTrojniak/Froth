#pragma once
#include "Vector3D.h"

namespace Froth::Math
{
	class Matrix3D
	{
	private:
		float m_Data[3][3];

	public:
		Matrix3D() = default;

		Matrix3D(float n00, float n01, float n02, float n10, float n11, float n12, float n20, float n21, float n22);

		Matrix3D(const Vector3D& a, const Vector3D& b, const Vector3D& c);

		float& operator ()(size_t i, size_t j);

		const float& operator ()(size_t i, size_t j) const;

		Vector3D& operator [](size_t j);

		const Vector3D& operator [](size_t j) const; 

		static float determinant(const Matrix3D& M);

		static Matrix3D inverse(const Matrix3D& M);

		static Matrix3D makeRotationX(float t);

		static Matrix3D makeRotationY(float t);

		static Matrix3D makeRotationZ(float t);

		//************************************
		// Method:    makeRotation
		// FullName:  Froth::Math::Matrix3D::makeRotation
		// Access:    public static 
		// Returns:   Froth::Math::Matrix3D
		// Qualifier:
		// Parameter: float t
		// Parameter: const Vector3D & v (unit length)
		//************************************
		static Matrix3D makeRotation(float t, const Vector3D& v);

		//************************************
		// Method:    makeReflection
		// FullName:  Froth::Math::Matrix3D::makeReflection
		// Access:    public static 
		// Returns:   Froth::Math::Matrix3D
		// Qualifier:
		// Parameter: const Vector3D & v (unit length)
		//************************************
		static Matrix3D makeReflection(const Vector3D& v);

		//************************************
		// Method:    makeInvolution
		// FullName:  Froth::Math::Matrix3D::makeInvolution
		// Access:    public static 
		// Returns:   Froth::Math::Matrix3D
		// Qualifier:
		// Parameter: const Vector3D & v (unit length)
		//************************************
		static Matrix3D makeInvolution(const Vector3D& v);

		static Matrix3D makeScale(float sx, float sy, float sz);

		//************************************
		// Method:    makeScale
		// FullName:  Froth::Math::Matrix3D::makeScale
		// Access:    public static 
		// Returns:   Froth::Math::Matrix3D
		// Qualifier:
		// Parameter: float s
		// Parameter: const Vector3D & v (unit length)
		//************************************
		static Matrix3D makeScale(float s, const Vector3D& v);

		static Matrix3D makeSkew(float t, const Vector3D& a, const Vector3D& b);
	};

	Matrix3D operator * (const Matrix3D& A, const Matrix3D& B);

	Vector3D operator * (const Matrix3D& M, const Vector3D& v);
}