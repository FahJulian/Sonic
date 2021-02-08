#pragma once

namespace Sonic {

	struct UISize
	{
		enum class Mode : uint8_t
		{
			Absolute, RelativeToEntity, RelativeToWindow
		};
	
		Mode mode;
		float relativeValue;
		float absoluteValue;
	};

}
