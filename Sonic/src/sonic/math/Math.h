#pragma once

namespace sonic
{
	template<typename T, typename F>
	inline bool min(const T& t, const F& f)
	{
		return (t < f) ? t : f;
	}

	template<typename T>
	inline bool min(std::initializer_list<T> values)
	{
		T* min = values.begin();

		for (T* it = values.begin(), end = values.end(); it < end; it++)
		{
			if (*it < *min)
				min = it;
		}

		return *min;
	}
}
