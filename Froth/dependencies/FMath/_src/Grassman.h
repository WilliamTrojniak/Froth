#pragma once
#include "Line.h"

namespace Froth::Math
{
	inline Line operator ^(const Point3D& p, const Point3D& q)
	{
		return (Line(q.x - p.x, q.y - p.y, q.z - p.z,
			p.y * q.z - p.z * q.y, p.z * q.x - p.x * q.z, p.x * q.y - p.y * q.x));
	}

	inline Line operator ^(const Plane& f, const Plane& g)
	{
		return (Line(f.z * g.y - f.y * g.z,
			f.x * g.z - f.z * g.x,
			f.y * g.x - f.x * g.y,
			f.x * g.w - f.w * g.x,
			f.y * g.w - f.w * g.y,
			f.z * g.w - f.w * g.z));
	}

	inline Plane operator ^(const Line& L, const Point3D& p)
	{
		return (Plane(L.m_Direction.y * p.z - L.m_Direction.z * p.y + L.m_Moment.x,
			L.m_Direction.z * p.x - L.m_Direction.x * p.z + L.m_Moment.y,
			L.m_Direction.x * p.y - L.m_Direction.y * p.x + L.m_Moment.z,
			-L.m_Moment.x * p.x - L.m_Moment.y * p.y - L.m_Moment.z * p.z));
	}

	inline Plane operator ^(const Point3D& p, const Line& L)
	{
		return (L ^ p);
	}

	inline Vector4D operator ^(const Line& L, const Plane& f)
	{
		return (Vector4D(
			L.m_Moment.y * f.z - L.m_Moment.z * f.y + L.m_Direction.x * f.w,
			L.m_Moment.z * f.x - L.m_Moment.x * f.z + L.m_Direction.y * f.w,
			L.m_Moment.x * f.y - L.m_Moment.y * f.x + L.m_Direction.z * f.w,
			-L.m_Direction.x * f.x - L.m_Direction.y * f.y - L.m_Direction.z * f.z));
	}

	inline Vector4D operator ^(const Plane& f, const Line& L)
	{
		return (L ^ f);
	}

	inline float operator ^(const Line& L1, const Line& L2)
	{
		return (-(Vector3D::dot(L1.m_Direction, L2.m_Moment) + Vector3D::dot(L2.m_Direction, L1.m_Moment)));
	}

	inline float operator ^(const Point3D& p, const Plane& f)
	{
		return (p.x * f.x + p.y * f.y + p.z * f.z + f.w);
	}

	inline float operator ^(const Plane& f, const Point3D& p)
	{
		return (-(p ^ f));
	}
}