#pragma once

namespace Sonic {

	struct UIPositionConstraintsComponent
	{
		float minX;
		float minY;
		float maxX;
		float maxY;

		UIPositionConstraintsComponent(float minX, float minY, float maxX, float maxY)
			: minX(minX), minY(minY), maxX(maxX), maxY(maxY)
		{
		}
	};

}
