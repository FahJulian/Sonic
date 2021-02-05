#pragma once


namespace Sonic {

	namespace Math {

		inline bool isInRange(float value, float minValue, float maxValue)
		{
			return value >= minValue && value < maxValue;
		}

	}

}
