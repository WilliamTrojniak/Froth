#include "Line.h"
#include "Matrix3D.h"

namespace Froth::Math
{

	Line::Line(float vx, float vy, float vz, float mx, float my, float mz)
		: m_Direction(vx, vy, vz), m_Moment(mx, my, mz)
	{

	}

	Line::Line(const Vector3D& v, const Vector3D& m)
		: m_Direction(v), m_Moment(m)
	{

	}

	Line Line::transform(const Line& line, const Transform4D& H)
	{
		Matrix3D adj(Vector3D::cross(H[1], H[2]), Vector3D::cross(H[2], H[0]), Vector3D::cross(H[0], H[1]));
		const Point3D& t = H.getTranslation();

		Vector3D v = H * line.m_Direction;
		Vector3D m = adj * line.m_Moment + Vector3D::cross(t, v);
		
		return Line(v, m);
	}

}