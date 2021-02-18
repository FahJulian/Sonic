#pragma once


namespace Sonic {

	namespace Math {

		template<typename T>
		inline bool isInRange(T value, T minValue, T maxValue)
		{
			return value >= minValue && value < maxValue;
		}

		template<typename T>
		inline T clamp(T value, T minValue, T maxValue)
		{
			return value < minValue ? minValue : value > maxValue ? maxValue : value;
		}

	}

}
