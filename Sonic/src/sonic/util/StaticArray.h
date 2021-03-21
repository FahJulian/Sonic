#pragma once

#include <utility>

#include "sonic/Base.h"

namespace sonic
{
	template<typename T, size_t N>
	class StaticArray
	{
	public:
		StaticArray() = default;

		StaticArray(const std::initializer_list<T>& data)
		{
			SN_ASSERT(data.size() <= N, "To many values for StaticArray");

			std::move(data.begin(), data.end(), mData);
		}

		T& operator[](size_t index) { return get(index); }
		const T& operator[](size_t index) const { return get(index); }

		T* begin()
		{
			return mData;
		}

		T* end()
		{
			return mData + N;
		}

		const T* begin() const
		{
			return mData;
		}

		const T* end() const
		{
			return mData + N;
		}

		template<typename F>
		void forEach(F function)
		{
			for (size_t i = 0; i < N; i++)
				function(mData[i]);
		}

		T& get(size_t index)
		{
			SN_ASSERT(index < N, "Array index out of range");

			return mData[index];
		}

		const T& get(size_t index) const
		{
			SN_ASSERT(index < N, "Array index out of range");

			return mData[index];
		}

		size_t getSize() const
		{
			return N;
		}

		T* getData()
		{
			return mData;
		}

		const T* getData() const
		{
			return mData;
		}

	private:
		T mData[N];
	};

}
