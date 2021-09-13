#pragma once
#include "Plane.h"
#include "Matrix4D.h"
#include "Transform4D.h"

namespace Froth::Math
{
	using deg = float;
	using rad = float;

	template <typename T> 
	int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	float toRadians(deg angle);

	float pointToLineDist(const Point3D& q, const Point3D& p, const Vector3D& v);

	float lineToLineDist(const Point3D& p1, const Vector3D& v1, const Point3D& p2, const Vector3D& v2);

	bool linePlaneIntersection(const Point3D& p, const Vector3D& v, const Plane& f, Point3D* q = nullptr);

	bool threePlaneIntersection(const Plane& f1, const Plane& f2, const Plane& f3, Point3D* p);

	bool twoPlaneIntersection(const Plane& f1, const Plane& f2, Point3D* p, Vector3D* v);

	Transform4D makeScale(float s);

	Transform4D makeScale(float x, float y, float z);
	
	Transform4D makeRotationX(rad angle);

	Transform4D makeRotationY(rad angle);

	Transform4D makeRotationZ(rad angle);

	Transform4D makeRotation(rad angle, Vector3D& a);

	Transform4D makeTranslation(float x, float y, float z);
	
	Matrix4D makeFrustumProjection(rad fovy, float s, float n, float f);

	Matrix4D makeInfiniteProjection(rad fovy, float s, float n, float e = 0.000001);

	Matrix4D makeRevFrustumProjection(rad fovy, float s, float n, float f);

	Matrix4D makeRevInfiniteProjection(rad fovy, float s, float n, float e);

	Matrix4D makeOrthoProjection(float l, float r, float t, float b, float n, float f);

	void modifyProjectionNearPlane(Matrix4D& P, const Plane& k);

	void modifyRevProjectionNearPlane(Matrix4D& R, const Plane& k);


}