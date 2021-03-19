#pragma once

namespace sonic
{
	template<typename T, size_t SIZE>
	class StaticArray
	{
	public:
		StaticArray() = default;

		T* begin()
		{
			return mData;
		}

		T* end()
		{
			return mData + SIZE;
		}

		size_t getSize()
		{
			return SIZE;
		}

	private:
		T mData[SIZE];
	};

}
