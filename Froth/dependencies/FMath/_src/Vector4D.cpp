#include "Vector4D.h"
#include <cmath>

namespace Froth::Math
{
	Vector4D::Vector4D(float a, float b, float c, float d)
		: x(a), y(b), z(c), w(d)
	{
	}

	Vector4D::Vector4D(const Vector3D& v)
		: x(v.x), y(v.y), z(v.z), w(0)
	{

	}

	Vector4D::Vector4D(const Point3D& p)
		: x(p.x), y(p.y), z(p.z), w(1)
	{

	}

	float Vector4D::operator[](size_t i)
	{
		// TODO: Check if index in range
		return ((&x)[i]);
	}

	const float Vector4D::operator[](size_t i) const
	{
		// TODO: Check if index in range
		return ((&x)[i]);
	}

	Vector4D& Vector4D::operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return (*this);
	}

	Vector4D& Vector4D::operator/=(float s)
	{
		s = 1.0f / s;
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return (*this);
	}

	Vector4D& Vector4D::operator+=(const Vector4D& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return (*this);
	}

	Vector4D& Vector4D::operator-=(const Vector4D& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return (*this);
	}

	float Vector4D::getMagnitude() const
	{
		return (sqrt(x * x + y * y + z * z + w * w));
	}

	float Vector4D::dot(const Vector4D& a, const Vector4D& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
	}

	Vector4D operator*(const Vector4D& v, float s)
	{
		return (Vector4D(v.x * s, v.y * s, v.z * s, v.w * s));
	}

	Froth::Math::Vector4D operator/(const Vector4D& v, float s)
	{
		s = 1.0F / s;
		return (Vector4D(v.x * s, v.y * s, v.z * s, v.w * s));
	}

	Froth::Math::Vector4D operator-(const Vector4D& v)
	{
		return (Vector4D(-v.x, -v.y, -v.z, -v.w));
	}

	Froth::Math::Vector4D operator+(const Vector4D& a, const Vector4D& b)
	{
		return (Vector4D(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w));
	}

	Froth::Math::Vector4D operator-(const Vector4D& a, const Vector4D& b)
	{
		return (Vector4D(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w));
	}

}