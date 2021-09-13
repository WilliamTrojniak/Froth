#pragma once
#include "Quaternion.h"
#include <cmath>

namespace Froth::Math
{
	Quaternion::Quaternion(float a, float b, float c, float s)
		: x(a), y(b), z(c), w(s)
	{
	}

	Quaternion::Quaternion(const Vector3D& v, float s)
		: x(v.x), y(v.y), z(v.z), w(s)
	{
	}

	const Vector3D& Quaternion::getVector() const
	{
		return reinterpret_cast<const Vector3D&>(x);
	}

	Matrix3D Quaternion::getRotationMatrix() const
	{
		// TODO: Check that quaternion has magnitude of 1
		
		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;
		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;

		return Matrix3D(
			1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy),
			2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx),
			2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2)
		);
	}

	void Quaternion::setRotationMatrix(const Matrix3D& M)
	{
		float m00 = M(0, 0);
		float m11 = M(1, 1);
		float m22 = M(2, 2);
		float sum = m00 + m11 + m22;

		if (sum > 0.0f)
		{
			w = sqrt(sum + 1.0f) * 0.5f;
			float f = 0.25f / w;

			x = (M(2, 1) - M(1, 2)) * f;
			y = (M(0, 2) - M(2, 0)) * f;
			z = (M(1, 0) - M(0, 1)) * f;
		}
		else if ((m00 > m11) && (m00 > m22))
		{
			x = sqrt(m00 - m11 - m22 + 1.0f) * 0.5f;
			float f = 0.25f / x;

			y = (M(1, 0) + M(0, 1)) * f;
			z = (M(0, 2) + M(2, 0)) * f;
			w = (M(2, 1) - M(1, 2)) * f;
		}
		else if (m11 > m22)
		{
			y = sqrt(m11 - m00 - m22 + 1.0f) * 0.5f;
			float f = 0.25f / y;

			x = (M(1, 0) + M(0, 1)) * f;
			z = (M(2, 1) + M(1, 2)) * f;
			w = (M(0, 2) - M(2, 0)) * f;
		}
		else
		{
			z = sqrt(m22 - m00 - m11 + 1.0f) * 0.5f;
			float f = 0.25f / z;

			x = (M(0, 2) + M(2, 0)) * f;
			y = (M(2, 1) + M(1, 2)) * f;
			w = (M(1, 0) - M(0, 1)) * f;
		}
	}

	Vector3D Quaternion::transformVector(const Vector3D& v, const Quaternion& q)
	{
		const Vector3D& b = q.getVector();
		float b2 = b.x * b.x + b.y * b.y + b.z * b.z;
		return (v * (q.w * q.w - b2) + b * (Vector3D::dot(v, b) * 2.0f) + Vector3D::cross(b, v) * (q.w * 2.0f));
	}

	Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
	{
		return Quaternion(
			q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
			q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
			q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
			q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
		);
	}

}