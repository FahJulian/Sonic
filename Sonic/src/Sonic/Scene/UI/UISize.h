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
	
		float absoluteValue = 0.0f;

		Mode mode = Mode::Absolute;
		float relativeValue = 0.0f;

		UISize() = default;

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
