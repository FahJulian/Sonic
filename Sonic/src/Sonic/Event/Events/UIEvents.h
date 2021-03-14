#pragma once
#include "Sonic/Window/Input/MouseButtons.h"

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

	struct UIEntityClickedEvent
	{
		float x;
		float y;
		float screenX;
		float screenY;
		MouseButton button;
	};

}
