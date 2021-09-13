#pragma once
#include "Vector4D.h"


namespace Froth::Math
{
	class Matrix4D
	{
	protected:
		float m_Data[4][4];

	public:
		Matrix4D() = default;

		explicit Matrix4D(float n);

		Matrix4D(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23, float n30, float n31, float n32, float n33);

		Matrix4D(const Vector4D& a, const Vector4D& b, const Vector4D& c, const Vector4D& d);

		float& operator ()(size_t i, size_t j);

		const float& operator ()(size_t i, size_t j) const;

		Vector4D& operator [](size_t j);

		const Vector4D& operator [](size_t j) const;

		//static Matrix4D inverse(const Matrix4D& M);

	};

	Matrix4D operator * (const Matrix4D& A, const Matrix4D& B);
	Vector4D operator * (const Matrix4D& M, const Vector4D& v);
}