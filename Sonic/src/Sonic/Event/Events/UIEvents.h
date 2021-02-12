#pragma once

namespace Sonic {

	struct UIEntityResizedEvent
	{
		float width;
		float height;
	};

	struct UIEntityMovedEvent
	{
		float deltaX;
		float deltaY;
	};

	struct UISliderEvent
	{
		float percentage;
	};

}
