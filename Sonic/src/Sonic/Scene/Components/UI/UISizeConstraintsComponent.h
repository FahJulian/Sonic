#pragma once

namespace Sonic {

	struct UISizeConstraintsComponent
	{
		float minWidth;
		float minHeight;
		float maxWidth;
		float maxHeight;

		UISizeConstraintsComponent(float minWidth, float minHeight, float maxWidth, float maxHeight)
			: minWidth(minWidth), minHeight(minHeight), maxWidth(maxWidth), maxHeight(maxHeight)
		{
		}
	};

}
