#pragma once
#include "Vec2.h"

namespace Sonic {
	
	struct Vec3
	{
		union
		{
			float x;
			float width;
		};

		union
		{
			float y;
			float height;
		};

		union
		{
			float z;
			float depth;
		};

		Vec3()
			: x(0.0f), y(0.0f), z(0.0f)
		{
		}

		Vec3(const Vec2& xy, float z)
			: x(xy.x), y(xy.y), z(z)
		{
		}

		Vec3(float x, float y, float z)
			: x(x), y(y), z(z)
		{
		}
	};

}
