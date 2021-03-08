#pragma once

namespace Sonic {

	struct UISize
	{
		enum class Mode : uint8_t
		{
			Absolute, 
			WindowCenter,
			RelativeToEntity, 
			RelativeToWindow,
		};
	
		float absoluteValue;

		Mode mode;
		float relativeValue;

		UISize(float value)
			: absoluteValue(value), mode(Mode::Absolute), relativeValue(value)
		{
		}

		UISize(UISize::Mode mode)
			: absoluteValue(0.0f), mode(mode), relativeValue(0.0f)
		{
		}

		UISize(Mode mode, float relativeValue)
			: absoluteValue(mode == Mode::Absolute ? relativeValue : 0.0f), mode(mode), relativeValue(relativeValue)
		{
		}
	};

}
