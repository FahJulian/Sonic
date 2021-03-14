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

		inline float toRadians(float angle)
		{
			return angle * 0.017453292519943295f;
		}

	}

}
