#include "Matrix3D.h"
#include <cmath>

namespace Froth::Math
{

	Matrix3D::Matrix3D(float n00, float n01, float n02, float n10, float n11, float n12, float n20, float n21, float n22)
	{
		m_Data[0][0] = n00; m_Data[0][1] = n10; m_Data[0][2] = n20;
		m_Data[1][0] = n01; m_Data[1][1] = n11; m_Data[1][2] = n21;
		m_Data[2][0] = n02; m_Data[2][1] = n12; m_Data[2][2] = n22;
	}

	Matrix3D::Matrix3D(const Vector3D& a, const Vector3D& b, const Vector3D& c)
	{
		m_Data[0][0] = a.x; m_Data[0][1] = a.y; m_Data[0][2] = a.z;
		m_Data[1][0] = b.x; m_Data[1][1] = b.y; m_Data[1][2] = b.z;
		m_Data[2][0] = c.x; m_Data[2][1] = c.y; m_Data[2][2] = c.z;
	}

	float& Matrix3D::operator()(size_t i, size_t j)
	{
		return (m_Data[j][i]);
	}


	const float& Matrix3D::operator()(size_t i, size_t j) const
	{
		return (m_Data[j][i]);
	}

	Vector3D& Matrix3D::operator[](size_t j)
	{
		return (*reinterpret_cast<Vector3D*>(m_Data[j]));
	}

	const Vector3D& Matrix3D::operator[](size_t j) const
	{
		return (*reinterpret_cast<const Vector3D*>(m_Data[j]));
	}

	float Matrix3D::determinant(const Matrix3D& M)
	{
		return (
			  M(0, 0) * (M(1, 1) * M(2, 2) - M(1, 2) * M(2, 1))
			+ M(0, 1) * (M(1, 2) * M(2, 0) - M(1, 0) * M(2, 2))
			+ M(0, 2) * (M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0))
		);
	}

	Matrix3D Matrix3D::inverse(const Matrix3D& M)
	{
		const Vector3D& a = M[0];
		const Vector3D& b = M[1];
		const Vector3D& c = M[2];

		Vector3D r0 = Vector3D::cross(b, c);
		Vector3D r1 = Vector3D::cross(c, a);
		Vector3D r2 = Vector3D::cross(a, b);

		float invDet = 1.0F / Vector3D::dot(r2, c);

		return Matrix3D(
			r0.x * invDet, r0.y * invDet, r0.z * invDet,
			r1.x * invDet, r1.y * invDet, r1.z * invDet,
			r2.x * invDet, r2.y * invDet, r2.z * invDet
		);
	}

	Matrix3D Matrix3D::makeRotationX(float t)
	{
		float c = cos(t);
		float s = sin(t);

		return Matrix3D(
			1.0F, 0.0F, 0.0F,
			0.0F, c, -s,
			0.0F, s, c
		);
	}

	Matrix3D Matrix3D::makeRotationY(float t)
	{
		float c = cos(t);
		float s = sin(t);

		return Matrix3D(
			c, 0.0f, s,
			0.0f, 1.0f, 0.0f,
			-s, 0.0f, c
		);
	}

	Matrix3D Matrix3D::makeRotationZ(float t)
	{
		float c = cos(t);
		float s = sin(t);

		return Matrix3D(
			c, -s, 0.0f,
			s, c, 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}

	Matrix3D Matrix3D::makeRotation(float t, const Vector3D& v)
	{
		// TODO: Check for unit length vector 
		float c = cos(t);
		float s = sin(t);
		float d = 1.0f - c;

		float x = v.x * d;
		float y = v.y * d;
		float z = v.z * d;

		float vxvy = x * v.y;
		float vxvz = x * v.z;
		float vyvz = y * v.z;

		return Matrix3D(
			c + x * v.x, vxvy - s * v.z, vxvz + s * v.y,
			vxvy + s * v.z, c + y * v.y, vyvz - s * v.x,
			vxvz - s * v.y, vyvz + s * v.x, c + z * v.z
		);
	}

	Matrix3D Matrix3D::makeReflection(const Vector3D& v)
	{
		// TODO: Check for unit length vector

		float x = v.x * -2.0f;
		float y = v.y * -2.0f;
		float z = v.z * -2.0f;

		float vxvy = x * v.y;
		float vxvz = x * v.z;
		float vyvz = y * v.z;

		return Matrix3D(
			x * v.x + 1.0f, vxvy, vxvz,
			vxvy, y * v.y + 1.0f, vyvz,
			vxvz, vyvz, z * v.z + 1.0f
		);
	}

	Matrix3D Matrix3D::makeInvolution(const Vector3D& v)
	{
		// TODO: Check for unit length vector

		float x = v.x * 2.0f;
		float y = v.y * 2.0f;
		float z = v.z * 2.0f;

		float vxvy = x * v.y;
		float vxvz = x * v.z;
		float vyvz = y * v.z;

		return Matrix3D(
			x * v.x - 1.0f, vxvy, vxvz,
			vxvy, y * v.y - 1.0f, vyvz,
			vxvz, vyvz, z * v.z - 1.0f
		);
	}

	Matrix3D Matrix3D::makeScale(float sx, float sy, float sz)
	{
		return Matrix3D(
			sx, 0.0f, 0.0f,
			0.0f, sy, 0.0f,
			0.0f, 0.0f, sz
		);
	}

	Matrix3D Matrix3D::makeScale(float s, const Vector3D& v)
	{
		s -= 1.0f;
		
		float x = v.x * s;
		float y = v.y * s;
		float z = v.z * s;

		float vxvy = x * v.y;
		float vxvz = x * v.z;
		float vyvz = y * v.z;

		return Matrix3D(
			x * v.x + 1.0f, vxvy, vxvz,
			vxvy, y * v.y + 1.0f, vyvz,
			vxvz, vyvz, z * v.z + 1.0f
		);
	}

	Matrix3D Matrix3D::makeSkew(float t, const Vector3D& a, const Vector3D& b)
	{
		t = tan(t);

		float x = a.x * t;
		float y = a.y * t;
		float z = a.z * t;

		return Matrix3D(
			x * b.x + 1.0f, x * b.y, x * b.z,
			y * b.x, y * b.y + 1.0f, y * b.z,
			z * b.x, z * b.y, z * b.z + 1.0f
		);
	}

	Matrix3D operator*(const Matrix3D& A, const Matrix3D& B)
	{
		return Matrix3D(
			A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0),
			A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1),
			A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2),
			A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0),
			A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1),
			A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2),
			A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0),
			A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1),
			A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2)
		);
	}

	Vector3D operator*(const Matrix3D& M, const Vector3D& v)
	{
		return Vector3D(
			M(0, 0) * v.x + M(0, 1) * v.y + M(0, 2) * v.z,
			M(1, 0) * v.x + M(1, 1) * v.y + M(1, 2) * v.z,
			M(2, 0) * v.x + M(2, 1) * v.y + M(2, 2) * v.z
		);
	}


}