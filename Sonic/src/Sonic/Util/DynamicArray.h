#pragma once

#include <limits>
#include <algorithm>
#include <initializer_list>

#include "Sonic/Base.h"

namespace Sonic {

	template<typename T>
	class DynamicArray
	{
	private:
		static constexpr double CAPACITY_MULTIPLIER = 1.15;

	public:
		constexpr DynamicArray()
			: DynamicArray(1)
		{
		}

		DynamicArray(size_t capacity)
			: mSize(0), mCapacity(capacity), mData(allocateMemory(capacity))
		{
		}

		DynamicArray(std::initializer_list<T> values)
			: DynamicArray(values.size())
		{
		}

		DynamicArray(const DynamicArray& other)
			: DynamicArray(other.mCapacity)
		{
		}

		~DynamicArray()
		{
			for (auto& value : *this)
				value.~T();

			freeMemory(mData);
		}

		template<typename... Args>
		void add(Args&&... args)
		{
			if (mSize == mCapacity)
				setCapacity(calculateNewCapacity());

			new(mData + mSize) T(std::forward<Args>(args)...);

			mSize++;
		}

		template<typename... Args>
		void operator+=(Args&&... args)
		{
			add(std::forward<Args>(args)...);
		}

		template<typename... Args>
		void insert(size_t index, Args&&... args)
		{
			SN_ASSERT(index <= mSize, "Cannot insert after end of DynamicArray");

			if (mSize == mCapacity)
			{
				mCapacity = calculateNewCapacity();

				T* newData = allocateMemory(mCapacity);
				std::move(mData, mData + index, newData);

				new(newData + index) T(std::forward<Args>(args)...);

				std::move(mData + index, mData + mSize, newData + index + 1);

				freeMemory(mData);
				mData = newData;
			}
			else
			{
				if (index != mSize)
					std::move_backward(mData + index, mData + mSize, mData + mSize + 1);

				new(mData + index) T(std::forward<Args>(args)...);
			}

			mSize++;
		}

		template<typename F>
		size_t indexOf(const F& value) const
		{
			for (size_t i = 0; i < mSize; i++)
			{
				if (mCursor == mSize)
					mCursor = 0;

				if (mData[mCursor] == value)
					return mCursor;

				mCursor++;
			}

			return mSize;
		}

		bool contains(const T& value) const
		{
			return indexOf(value) != mSize;
		}

		void remove(size_t index)
		{
			SN_ASSERT(index < mSize, "Index ", index, " is out of range");

			mData[index].~T();
			std::move(mData + index + 1, mData + mSize, mData + index);

			mSize--;
		}

		void setSize(size_t size)
		{
			if (size > mCapacity)
				setCapacity(size);

			if (size > mSize)
				new(mData + mSize) T[size - mSize];

			mSize = size;
		}

		void setCapacity(size_t capacity)
		{
			if (capacity < mSize)
			{
				for (size_t i = capacity; i < mSize; i++)
					mData[i].~T();

				mSize = capacity;
			}

			mCapacity = capacity;

			T* newData = allocateMemory(mCapacity);
			std::move(begin(), end(), newData);

			freeMemory(mData);
			mData = newData;
		}

		void clear()
		{
			for (auto& value : *this)
				value.~T();

			mSize = 0;
		}

		T* begin()
		{
			return mData;
		}

		T* end()
		{
			return mData + mSize;
		}

		const T* cbegin() const
		{
			return mData;
		}

		const T* cend() const
		{
			return mData + mSize;
		}

		T* getData()
		{
			return mData;
		}

		const T* getData() const
		{
			return mData;
		}

		size_t getSize() const
		{
			return mSize;
		}

		size_t getCapacity() const
		{
			return mCapacity;
		}

		T& get(size_t index)
		{
			SN_ASSERT(index < mSize, "Index ", index, " is out of range");

			return mData[index];
		}

		const T& get(size_t index) const
		{
			SN_ASSERT(index < mSize, "Index ", index, " is out of range");

			return mData[index];
		}

		T& operator[](size_t index)
		{
			SN_ASSERT(index < mSize, "Index ", index, " is out of range");

			return mData[index];
		}

		const T& operator[](size_t index) const
		{
			SN_ASSERT(index < mSize, "Index ", index, " is out of range");

			return mData[index];
		}

	private:
		inline static T* allocateMemory(size_t capacity)
		{
			return reinterpret_cast<T*>(operator new(capacity * sizeof(T)));
		}

		inline static void freeMemory(T* data)
		{
			operator delete((void*)data);
		}

		inline size_t calculateNewCapacity()
		{
			return static_cast<size_t>(static_cast<double>(mCapacity) * CAPACITY_MULTIPLIER + 0.5);
		}

		size_t mSize;
		size_t mCapacity;
		mutable size_t mCursor = 0;
		T* mData;
	};

}
