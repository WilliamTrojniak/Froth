#include "Vector3D.h"
#include <cmath>

namespace Froth::Math
{
	Vector3D::Vector3D(float n)
		: x(n), y(n), z(n)
	{
	}
	Vector3D::Vector3D(float a, float b, float c)
		: x(a), y(b), z(c)
	{
	}

	float Vector3D::operator[](size_t i)
	{
		// TODO: Check if index in range
		return ((&x)[i]);
	}

	const float Vector3D::operator[](size_t i) const
	{
		// TODO: Check if index in range
		return ((&x)[i]);
	}

	Vector3D& Vector3D::operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return (*this);
	}

	Vector3D& Vector3D::operator/=(float s)
	{
		s = 1.0f / s;
		x *= s;
		y *= s;
		z *= s;
		return (*this);
	}

	Vector3D& Vector3D::operator+=(const Vector3D& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return (*this);
	}

	Vector3D& Vector3D::operator-=(const Vector3D& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return (*this);
	}

	float Vector3D::getMagnitude() const
	{
		return (sqrt(x * x + y * y + z * z));
	}

	Vector3D Vector3D::normalize(const Vector3D& v)
	{
		return (v / v.getMagnitude());
	}

	float Vector3D::dot(const Vector3D& a, const Vector3D& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	Vector3D Vector3D::cross(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	Vector3D Vector3D::project(const Vector3D& a, const Vector3D& b)
	{
		return (b * (dot(a, b) / dot(b, b)));
	}

	Vector3D Vector3D::reject(const Vector3D& a, const Vector3D& b)
	{
		return (a - b * (dot(a, b) / dot(b, b)));
	}

	Vector3D operator*(const Vector3D& v, float s)
	{
		return (Vector3D(v.x * s, v.y * s, v.z * s));
	}

	Vector3D operator/(const Vector3D& v, float s)
	{
		s = 1.0F / s;
		return (Vector3D(v.x * s, v.y * s, v.z * s));
	}

	Vector3D operator-(const Vector3D& v)
	{
		return (Vector3D(-v.x, -v.y, -v.z));
	}

	Vector3D operator+(const Vector3D& a, const Vector3D& b)
	{
		return (Vector3D(a.x + b.x, a.y + b.y, a.z + b.z));
	}

	Vector3D operator-(const Vector3D& a, const Vector3D& b)
	{
		return (Vector3D(a.x - b.x, a.y - b.y, a.z - b.z));
	}

}