#include "Vector2D.h"
#include <cmath>
namespace Froth::Math
{

	Vector2D::Vector2D(float a, float b)
		: x(a), y(b)
	{
	}

	float Vector2D::operator[](size_t i)
	{
		// TODO: Check if index in range
		return((&x)[i]);
	}

	const float Vector2D::operator[](size_t i) const
	{
		// TODO: Check if index in range
		return((&x)[i]);
	}

	Vector2D& Vector2D::operator*=(float s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	Vector2D& Vector2D::operator/=(float s)
	{
		s = 1.0f / s;
		x *= s;
		y *= s;
		return *this;
	}

	Vector2D& Vector2D::operator+=(const Vector2D& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2D& Vector2D::operator-=(const Vector2D& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	float Vector2D::getMagnitude() const
	{
		return (sqrt(x * x + y * y));
	}

	Vector2D Vector2D::normalize(const Vector2D& v)
	{
		return (v / v.getMagnitude());
	}

	float Vector2D::dot(const Vector2D& a, const Vector2D& b)
	{
		return (a.x * b.x + a.y * b.y);
	}

	Vector2D operator*(const Vector2D& v, float s)
	{
		return Vector2D(v.x * s, v.y * s);
	}

	Vector2D operator/(const Vector2D& v, float s)
	{
		s = 1.0f / s;
		return Vector2D(v.x * s, v.y * s);
	}

	Vector2D operator-(const Vector2D& v)
	{
		return Vector2D(-v.x, -v.y);
	}

	Vector2D operator+(const Vector2D& a, const Vector2D& b)
	{
		return Vector2D(a.x + b.x, a.y + b.y);
	}

	Vector2D operator-(const Vector2D& a, const Vector2D& b)
	{
		return Vector2D(a.x - b.x, a.y - b.y);
	}

}


