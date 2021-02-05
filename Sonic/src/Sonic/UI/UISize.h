#pragma once

namespace Sonic {

	struct UISize
	{
		enum class Mode : uint8_t
		{
			Absolute, RelativeToEntity, RelativeToWindow
		};
	
		Mode mode;

		union
		{
			float relativeValue;
			float changedValue;
		};

		float absoluteValue;
	};

}
