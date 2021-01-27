#pragma once

namespace Sonic {

	struct UISize
	{
		enum class Mode : unsigned char
		{
			Absolute, RelativeToEntity, RelativeToWindow
		};
	
		Mode mode;
		float value;
	};

}
