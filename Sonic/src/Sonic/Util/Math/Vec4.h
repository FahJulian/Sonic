#pragma once
#include "Vec2.h"
#include "Vec3.h"

namespace Sonic {

	struct Vec4
	{
		float x;
		float y;
		float z;
		float w;

		Vec4()
			: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{
		}

		Vec4(const Vec2& xy, float z, float w)
			: x(xy.x), y(xy.y), z(z), w(w)
		{
		}

		Vec4(const Vec2& xy, const Vec2& zw)
			: x(xy.x), y(xy.y), z(zw.x), w(zw.y)
		{
		}

		Vec4(const Vec3& xyz, float w)
			: x(xyz.x), y(xyz.y), z(xyz.z), w(w)
		{
		}

		Vec4(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w)
		{
		}
	};

}
