#pragma once
#include "Transform4D.h"

namespace Froth::Math
{
	class Line
	{
	public:
		Vector3D m_Direction;
		Vector3D m_Moment;

		Line() = default;

		Line(float vx, float vy, float vz, float mx, float my, float mz);

		Line(const Vector3D& v, const Vector3D& m);

		static Line transform(const Line& line, const Transform4D& H);
	};
}
