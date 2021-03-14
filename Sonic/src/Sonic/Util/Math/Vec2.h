#pragma once

namespace Sonic {
	
	struct Vec2
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

		Vec2()
			: x(0.0f), y(0.0f)
		{
		}

		Vec2(float x, float y)
			: x(x), y(y)
		{
		}


	};

}
