#include "Matrix4D.h"
#include "Vector3D.h"

namespace Froth::Math
{
	
	Matrix4D::Matrix4D(float n)
	{
		m_Data[0][0] = n;	 m_Data[0][1] = 0.0f; m_Data[0][2] = 0.0f; m_Data[0][3] = 0.0f;
		m_Data[1][0] = 0.0f; m_Data[1][1] = n;	  m_Data[1][2] = 0.0f; m_Data[1][3] = 0.0f;
		m_Data[2][0] = 0.0f; m_Data[2][1] = 0.0f; m_Data[2][2] = n;	   m_Data[2][3] = 0.0f;
		m_Data[3][0] = 0.0f; m_Data[3][1] = 0.0f; m_Data[3][2] = 0.0f; m_Data[3][3] = n;
	}

	
	Matrix4D::Matrix4D(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23, float n30, float n31, float n32, float n33)
	{
		m_Data[0][0] = n00; m_Data[0][1] = n10; m_Data[0][2] = n20; m_Data[0][3] = n30;
		m_Data[1][0] = n01; m_Data[1][1] = n11; m_Data[1][2] = n21; m_Data[1][3] = n31;
		m_Data[2][0] = n02; m_Data[2][1] = n12; m_Data[2][2] = n22; m_Data[2][3] = n32;
		m_Data[3][0] = n03; m_Data[3][1] = n13; m_Data[3][2] = n23; m_Data[3][3] = n33;
	}

	Matrix4D::Matrix4D(const Vector4D& a, const Vector4D& b, const Vector4D& c, const Vector4D& d)
	{
		m_Data[0][0] = a.x; m_Data[0][1] = a.y; m_Data[0][2] = a.z; m_Data[0][3] = a.w;
		m_Data[1][0] = b.x; m_Data[1][1] = b.y; m_Data[1][2] = b.z; m_Data[1][3] = b.w;
		m_Data[2][0] = c.x; m_Data[2][1] = c.y; m_Data[2][2] = c.z; m_Data[2][3] = c.w;
		m_Data[3][0] = d.x; m_Data[3][1] = d.y; m_Data[3][2] = d.z; m_Data[3][3] = d.w;
	}

	float& Matrix4D::operator()(size_t i, size_t j)
	{
		return (m_Data[j][i]);
	}

	const float& Matrix4D::operator()(size_t i, size_t j) const
	{	
		return (m_Data[j][i]);
	}

	Vector4D& Matrix4D::operator[](size_t j)
	{
		return (*reinterpret_cast<Vector4D*>(m_Data[j]));
	}

	const Vector4D& Matrix4D::operator[](size_t j) const
	{
		return (*reinterpret_cast<const Vector4D*>(m_Data[j]));
	}
	/*
	Matrix4D Matrix4D::inverse(const Matrix4D& M)
	{
		
		const Vector3D& a = reinterpret_cast<const Vector3D&>(M[0]);
		const Vector3D& b = reinterpret_cast<const Vector3D&>(M[1]);
		const Vector3D& c = reinterpret_cast<const Vector3D&>(M[2]);
		const Vector3D& d = reinterpret_cast<const Vector3D&>(M[3]);
		
		const float& x = M(3, 0);
		const float& y = M(3, 1);
		const float& z = M(3, 2);
		const float& w = M(3, 3);
		
		Vector3D s = Vector3D::cross(a, b);
		Vector3D t = Vector3D::cross(c, d);
		Vector3D u = a * y - b * x;
		Vector3D v = c * w - d * z;
		
		float invDet = 1.0F / (Vector3D::dot(s, v) + Vector3D::dot(t, u));
		s *= invDet;
		t *= invDet;
		u *= invDet;
		v *= invDet;

		Vector3D r0 = Vector3D::cross(b, v) + t * y;
		Vector3D r1 = Vector3D::cross(v, a) - t * x;
		Vector3D r2 = Vector3D::cross(d, u) + s * w;
		Vector3D r3 = Vector3D::cross(u, c) - s * z;

		return (Matrix4D(
				r0.x, r0.y, r0.z, -Vector3D::dot(b, t),
				r1.x, r1.y, r1.z, Vector3D::dot(a, t),
				r2.x, r2.y, r2.z, -Vector3D::dot(d, s),
				r3.x, r3.y, r3.z, Vector3D::dot(c, s))
		); 
	}
	*/
	

	Matrix4D operator*(const Matrix4D& A, const Matrix4D& B)
	{
		return Matrix4D(
			A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0) + A(0, 3) * B(3, 0),
			A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1) + A(0, 3) * B(3, 1),
			A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2) + A(0, 3) * B(3, 2),
			A(0, 0) * B(0, 3) + A(0, 1) * B(1, 3) + A(0, 2) * B(2, 3) + A(0, 3) * B(3, 3), 
			A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0) + A(1, 3) * B(3, 0),
			A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1) + A(1, 3) * B(3, 1),
			A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2) + A(1, 3) * B(3, 2),
			A(1, 0) * B(0, 3) + A(1, 1) * B(1, 3) + A(1, 2) * B(2, 3) + A(1, 3) * B(3, 3),
			A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0) + A(2, 3) * B(3, 0),
			A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1) + A(2, 3) * B(3, 1),
			A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2) + A(2, 3) * B(3, 2),
			A(2, 0) * B(0, 3) + A(2, 1) * B(1, 3) + A(2, 2) * B(2, 3) + A(2, 3) * B(3, 3),
			A(3, 0) * B(0, 0) + A(3, 1) * B(1, 0) + A(3, 2) * B(2, 0) + A(3, 3) * B(3, 0),
			A(3, 0) * B(0, 1) + A(3, 1) * B(1, 1) + A(3, 2) * B(2, 1) + A(3, 3) * B(3, 1),
			A(3, 0) * B(0, 2) + A(3, 1) * B(1, 2) + A(3, 2) * B(2, 2) + A(3, 3) * B(3, 2),
			A(3, 0) * B(0, 3) + A(3, 1) * B(1, 3) + A(3, 2) * B(2, 3) + A(3, 3) * B(3, 3)
		);
	}

	Vector4D operator*(const Matrix4D& M, const Vector4D& v)
	{
		return Vector4D(
			M(0, 0) * v.x + M(0, 1) * v.y + M(0, 2) * v.z + M(0, 3) * v.w,
			M(1, 0) * v.x + M(1, 1) * v.y + M(1, 2) * v.z + M(1, 3) * v.w,
			M(2, 0) * v.x + M(2, 1) * v.y + M(2, 2) * v.z + M(2, 3) * v.w,
			M(3, 0) * v.x + M(3, 1) * v.y + M(3, 2) * v.z + M(3, 3) * v.w
		);
	}

}