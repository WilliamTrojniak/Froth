#pragma once
namespace Froth::Math
{
	class Vector2D
	{
	public:
		float x, y;

		Vector2D() = default;

		Vector2D(float a, float b);


		float operator [](size_t i);

		const float operator [](size_t i) const;

		Vector2D& operator *= (float s);

		Vector2D& operator /= (float s);

		Vector2D& operator += (const Vector2D& v);

		Vector2D& operator -= (const Vector2D& v);

		inline float getMagnitude() const;

		static Vector2D normalize(const Vector2D& v);

		static float dot(const Vector2D& a, const Vector2D& b);

	};

	Vector2D operator * (const Vector2D& v, float s);
								
	Vector2D operator / (const Vector2D& v, float s);
								
	Vector2D operator - (const Vector2D& v);
								
	Vector2D operator + (const Vector2D& a, const Vector2D& b);
	
	Vector2D operator - (const Vector2D& a, const Vector2D& b);
}