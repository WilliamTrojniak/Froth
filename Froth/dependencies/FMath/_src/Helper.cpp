#pragma once
#define _USE_MATH_DEFINES
#include "Helper.h"
#include <cmath>
#include <cfloat>

namespace Froth::Math
{
	float toRadians(deg angle)
	{
		return angle * M_PI / 180.0f;
	}

	float pointToLineDist(const Point3D& q, const Point3D& p, const Vector3D& v)
	{
		Vector3D a = Vector3D::cross(q - p, v);
		return (sqrt(Vector3D::dot(a, a) / Vector3D::dot(v, v)));
	}

	float lineToLineDist(const Point3D& p1, const Vector3D& v1, const Point3D& p2, const Vector3D& v2)
	{
		Vector3D dp = p2 - p1;

		float v12 = Vector3D::dot(v1, v1);
		float v22 = Vector3D::dot(v2, v2);
		float v1v2 = Vector3D::dot(v1, v2);

		float det = v1v2 * v1v2 - v12 * v22;

		if (fabs(det) > FLT_MIN)
		{
			det = 1.0f / det;

			float dpv1 = Vector3D::dot(dp, v1);
			float dpv2 = Vector3D::dot(dp, v2);
			float t1 = (v1v2 * dpv2 - v22 * dpv1) * det;
			float t2 = (v12 * dpv2 - v1v2 * dpv1) * det;

			return (dp + v2 * t2 - v1 * t1).getMagnitude();
		}

		Vector3D a = Vector3D::cross(dp, v1);
		return sqrt(Vector3D::dot(a, a) / v12);
	}

	bool linePlaneIntersection(const Point3D& p, const Vector3D& v, const Plane& f, Point3D* q /*= nullptr*/)
	{
		float fv = Plane::dot(f, v);
		if (fabs(fv) > FLT_MIN)
		{
			*q = p - v * (Plane::dot(f, p) / fv);
			return true;
		}

		return false;
	}

	bool threePlaneIntersection(const Plane& f1, const Plane& f2, const Plane& f3, Point3D* p)
	{
		const Vector3D& n1 = f1.getNormal();
		const Vector3D& n2 = f2.getNormal();
		const Vector3D& n3 = f2.getNormal();

		Vector3D n1xn2 = Vector3D::cross(n1, n2);
		float det = Vector3D::dot(n1xn2, n3);
		if (fabs(det) > FLT_MIN)
		{
			*p = Point3D((Vector3D::cross(n3, n2) * f1.w + Vector3D::cross(n1, n3) * f2.w - n1xn2 * f3.w) / det);
			return true;
		}

		return false;
	}

	bool twoPlaneIntersection(const Plane& f1, const Plane& f2, Point3D* p, Vector3D* v)
	{
		const Vector3D& n1 = f1.getNormal();
		const Vector3D& n2 = f2.getNormal();

		*v = Vector3D::cross(n1, n2);
		float det = Vector3D::dot(*v, *v);

		if (fabs(det) > FLT_MIN)
		{
			*p = Point3D(Vector3D::cross(*v, n2) * f1.w + Vector3D::cross(n1, *v) * f2.w / det);
			return true;
		}

		return false;
	}


	Transform4D makeScale(float s)
	{
		return Transform4D(
			   s, 0.0f, 0.0f, 0.0f,
			0.0f,    s, 0.0f, 0.0f,
			0.0f, 0.0f,    s, 0.0f
		);
	}

	Transform4D makeScale(float x, float y, float z)
	{
		return Transform4D(
			   x, 0.0f, 0.0f, 0.0f,
			0.0f,    y, 0.0f, 0.0f,
			0.0f, 0.0f,    z, 0.0f
		);
	}

	Transform4D makeRotationX(rad angle)
	{
		float c = cos(angle);
		float s = sin(angle);

		return Transform4D(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,   -s, 0.0f,
			0.0f,    s,    c, 0.0f
		);
	}

	Transform4D makeRotationY(rad angle)
	{
		float c = cos(angle);
		float s = sin(angle);

		return Transform4D(
			   c, 0.0f,    s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			  -s, 0.0f,    c, 0.0f
		);
	}

	Transform4D makeRotationZ(rad angle)
	{
		float c = cos(angle);
		float s = sin(angle);

		return Transform4D(
			   c,   -s, 0.0f, 0.0f,
			   s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f
		);
	}

	Transform4D makeRotation(rad angle, Vector3D& a)
	{
		float c = cos(angle);
		float s = sin(angle);
		float d = 1.0f - c;

		float x = a.x * d;
		float y = a.y * d;
		float z = a.z * d;

		float axay = x * a.y;
		float axaz = x * a.z;
		float ayaz = y * a.z;

		return Transform4D(
			c + x * a.x, axay - s * a.z, axaz + s * a.y, 0.0f,
			axay + s * a.z, c + y * a.y, ayaz - s * a.x, 0.0f,
			axaz - s * a.y, ayaz + s * a.x, c + z * a.z, 0.0f
		);
	}

	Transform4D makeTranslation(float x, float y, float z)
	{
		return Transform4D(
			1.0f, 0.0f, 0.0f, x,
			0.0f, 1.0f, 0.0f, y,
			0.0f, 0.0f, 1.0f, z
		);
	}

	Matrix4D makeFrustumProjection(rad fovy, float s, float n, float f)
	{
		float g = 1.0f / tan(fovy * 0.5f);
		float k = f / (f - n);

		return Matrix4D(
			g / s, 0.0f, 0.0f, 0.0f,
			0.0f, g, 0.0f, 0.0f,
			0.0f, 0.0f, k, -n * k,
			0.0f, 0.0f, 1.0f, 0.0f
		);
	}

	Matrix4D makeInfiniteProjection(rad fovy, float s, float n, float e)
	{
		float g = 1.0f / tan(fovy * 0.5f);
		e = 1.0f - e;

		return Matrix4D(
			g / s, 0.0f, 0.0f, 0.0f,
			0.0f, g, 0.0f, 0.0f,
			0.0f, 0.0f, e, -n * e,
			0.0f, 0.0f, 1.0f, 0.0f
		);
	}

	Matrix4D makeRevFrustumProjection(rad fovy, float s, float n, float f)
	{
		float g = 1.0f / tan(fovy * 0.5f);
		float k = n / (n - f);

		return Matrix4D(
			g / s, 0.0f, 0.0f, 0.0f,
			0.0f, g, 0.0f, 0.0f,
			0.0f, 0.0f, k, -f * k,
			0.0f, 0.0f, 1.0f, 0.0f
		);
	}

	Matrix4D makeRevInfiniteProjection(rad fovy, float s, float n, float e)
	{
		float g = 1.0f / tan(fovy * 0.5f);

		return Matrix4D(
			g / s, 0.0f, 0.0f, 0.0f,
			0.0f, g, 0.0f, 0.0f,
			0.0f, 0.0f, e, n * (1.0f - e),
			0.0f, 0.0f, 1.0f, 0.0f
		);
	}

	Matrix4D makeOrthoProjection(float l, float r, float t, float b, float n, float f)
	{
		float w_inv = 1.0f / (r - l);
		float h_inv = 1.0f / (b - t);
		float d_inv = 1.0f / (f - n);

		return Matrix4D(
			2.0f * w_inv, 0.0f, 0.0f, -(r + l) * w_inv,
			0.0f, 2.0f * h_inv, 0.0f, -(b + t) * h_inv,
			0.0f, 0.0f, d_inv, -n * d_inv,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	void modifyProjectionNearPlane(Matrix4D& P, const Plane& k)
	{
		Vector4D vcamera(
			(sgn(k.x) - P(0, 2)) / P(0, 0),
			(sgn(k.y) - P(1, 2)) / P(1, 1),
			1.0f,
			P(2, 2) / P(2, 3)
		);

		float m = 1.0f / Plane::dot(k, vcamera);
		P(2, 0) = m * k.x;
		P(2, 1) = m * k.y;
		P(2, 2) = m * k.z;
		P(2, 3) = m * k.w;
	}

	void modifyRevProjectionNearPlane(Matrix4D& R, const Plane& k)
	{
		Vector4D vcamera(
			(sgn(k.x) - R(0, 2)) / R(0, 0),
			(sgn(k.y) - R(1, 2)) / R(1, 1),
			1.0f,
			R(2, 2) / R(2, 3)
		);

		float m = -1.0f / Plane::dot(k, vcamera);
		R(2, 0) = m * k.x;
		R(2, 1) = m * k.y;
		R(2, 2) = m * k.z + 1.0f;
		R(2, 3) = m * k.w;
	}

	

}