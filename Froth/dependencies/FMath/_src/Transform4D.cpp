#pragma once
#include "Transform4D.h"

namespace Froth::Math
{
	Transform4D::Transform4D(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23)
	{
		m_Data[0][0] = n00; m_Data[0][1] = n10; m_Data[0][2] = n20; 
		m_Data[1][0] = n01; m_Data[1][1] = n11; m_Data[1][2] = n21; 
		m_Data[2][0] = n02; m_Data[2][1] = n12; m_Data[2][2] = n22;
		m_Data[3][0] = n03; m_Data[3][1] = n13; m_Data[3][2] = n23;

		m_Data[0][3] = m_Data[1][3] = m_Data[2][3] = 0.0f;
		m_Data[3][3] = 1.0f;
	}

	Transform4D::Transform4D(const Vector3D& a, const Vector3D& b, const Vector3D& c, const Point3D& p)
	{
		m_Data[0][0] = a.x; m_Data[0][1] = a.y; m_Data[0][2] = a.z;
		m_Data[1][0] = b.x; m_Data[1][1] = b.y; m_Data[1][2] = b.z;
		m_Data[2][0] = c.x; m_Data[2][1] = c.y; m_Data[2][2] = c.z;
		m_Data[3][0] = p.x; m_Data[3][1] = p.y; m_Data[3][2] = p.z;

		m_Data[0][3] = m_Data[1][3] = m_Data[2][3] = 0.0f;
		m_Data[3][3] = 1.0f;
	}

	Vector3D& Transform4D::operator[](size_t j)
	{
		return (*reinterpret_cast<Vector3D*>(m_Data[j]));
	}

	const Vector3D& Transform4D::operator[](size_t j) const
	{
		return (*reinterpret_cast<const Vector3D*>(m_Data[j]));
	}

	const Point3D& Transform4D::getTranslation() const
	{
		return (*reinterpret_cast<const Point3D*>(m_Data[3]));
	}

	void Transform4D::setTranslation(const Point3D& p)
	{
		m_Data[3][0] = p.x;
		m_Data[3][1] = p.y;
		m_Data[3][2] = p.z;
	}

	Transform4D Transform4D::inverse(const Transform4D& H)
	{
		const Vector3D& a = H[0];
		const Vector3D& b = H[1];
		const Vector3D& c = H[2];
		const Vector3D& d = H[3];

		Vector3D s = Vector3D::cross(a, b);
		Vector3D t = Vector3D::cross(c, d);

		float invDet = 1.0f / Vector3D::dot(s, c);

		s *= invDet;
		t *= invDet;
		Vector3D v = c * invDet;

		Vector3D r0 = Vector3D::cross(b, v);
		Vector3D r1 = Vector3D::cross(v, a);

		return Transform4D(
			r0.x, r0.y, r0.z, -Vector3D::dot(b, t),
			r1.x, r1.y, r1.z, Vector3D::dot(a, t),
			s.x, s.y, s.z, -Vector3D::dot(d, s)
		);
	}

	Transform4D Transform4D::makeReflection(const Plane& f)
	{
		float x = f.x * -2.0f;
		float y = f.y * -2.0f;
		float z = f.z * -2.0f;

		float nxny = x * f.y;
		float nxnz = x * f.z;
		float nynz = y * f.z;

		return Transform4D(
			x * f.x + 1.0f, nxny, nxnz, x * f.w,
			nxny, y * f.y + 1.0f, nynz, y * f.w,
			nxnz, nynz, z * f.z + 1.0f, z * f.w
		);
	}

	Transform4D operator*(const Transform4D& A, const Transform4D& B)
	{
		return Transform4D(
			A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0),
			A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1),
			A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2),
			A(0, 0) * B(0, 3) + A(0, 1) * B(1, 3) + A(0, 2) * B(2, 3) + A(0, 3),
			A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0),
			A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1),
			A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2),
			A(1, 0) * B(0, 3) + A(1, 1) * B(1, 3) + A(1, 2) * B(2, 3) + A(1, 3),
			A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0),
			A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1),
			A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2),
			A(2, 0) * B(0, 3) + A(2, 1) * B(1, 3) + A(2, 2) * B(2, 3) + A(2, 3)
		);
	}

	Vector3D operator*(const Transform4D& H, const Vector3D& v)
	{
		return Vector3D(
			H(0, 0) * v.x + H(0, 1) * v.y + H(0, 2) * v.z,
			H(1, 0) * v.x + H(1, 1) * v.y + H(1, 2) * v.z,
			H(2, 0) * v.x + H(2, 1) * v.y + H(2, 2) * v.z
		);
	}

	Froth::Math::Vector3D operator*(const Vector3D& n, const Transform4D& H)
	{
		return Vector3D(
			n.x * H(0, 0) + n.y * H(1, 0) + n.z * H(2, 0),
			n.x * H(0, 1) + n.y * H(1, 1) + n.z * H(2, 1),
			n.x * H(0, 2) + n.y * H(1, 2) + n.z * H(2, 2)
		);
	}

	Point3D operator*(const Transform4D& H, const Point3D& p)
	{
		return Point3D(
			H(0, 0) * p.x + H(0, 1) * p.y + H(0, 2) * p.z + H(0, 3),
			H(1, 0) * p.x + H(1, 1) * p.y + H(1, 2) * p.z + H(1, 3),
			H(2, 0) * p.x + H(2, 1) * p.y + H(2, 2) * p.z + H(2, 3)
		);
	}
	
	Plane operator*(const Plane& f, const Transform4D& H)
	{
		return Plane(
			f.x * H(0, 0) + f.y * H(1, 0) + f.z * H(2, 0),
			f.x * H(0, 1) + f.y * H(1, 1) + f.z * H(2, 1),
			f.x * H(0, 2) + f.y * H(1, 2) + f.z * H(2, 2),
			f.x * H(0, 3) + f.y * H(1, 3) + f.z * H(2, 3) + f.w
		);
	}


}