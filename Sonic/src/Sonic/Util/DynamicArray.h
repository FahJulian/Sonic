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
		static constexpr double CAPACITY_MULTIPLIER_FACTOR = 1.15;

	public:
		constexpr DynamicArray()
			: mSize(0), mCapacity(0), mData(nullptr)
		{
		}

		DynamicArray(size_t capacity)
			: mSize(0), mCapacity(capacity), mData(allocateMemory(capacity))
		{
		}

		DynamicArray(const std::initializer_list<T>& values)
			: mSize(values.size()), mCapacity(values.size()), mData(allocateMemory(mCapacity))
		{
			for (size_t i = 0, size = values.size(); i < size; i++)
				new(mData + i) T(std::move(*(values.begin() + i)));
		}

		DynamicArray(const DynamicArray& other)
			: mSize(0), mCapacity(0), mData(nullptr)
		{
			*this = other;
		}

		DynamicArray(DynamicArray&& other) noexcept
			: mSize(0), mCapacity(0), mData(nullptr)
		{
			*this = std::move(other);
		}

		DynamicArray& operator=(const DynamicArray& other)
		{
			if (this != &other)
			{
				for (auto& value : *this)
					value.~T();

				freeMemory(mData);

				mCapacity = other.mCapacity;
				mSize = other.mSize;
				mCursor = other.mCursor;
				mData = allocateMemory(mCapacity);

				for (size_t i = 0; i < other.mSize; i++)
					new(mData + i) T(other.get(i));
			}

			return *this;
		}

		DynamicArray& operator=(DynamicArray&& other) noexcept
		{
			if (this != &other)
			{
				for (auto& value : *this)
					value.~T();

				freeMemory(mData);

				mCapacity = other.mCapacity;
				mSize = other.mSize;
				mCursor = other.mCursor;
				mData = other.mData;

				other.mCapacity = 0;
				other.mSize = 0;
				other.mData = nullptr;
				other.mCursor = 0;
			}

			return *this;
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

		void add(const T& value)
		{
			if (mSize == mCapacity)
				setCapacity(calculateNewCapacity());

			new(mData + mSize) T(value);

			mSize++;
		}

		template<typename... Args>
		void operator+=(Args&&... args)
		{
			add(std::forward<Args>(args)...);
		}

		void operator+=(const T& value)
		{
			add(value);
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

		template<typename F>
		bool contains(const F& value) const
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
			if (size > mSize)
			{
				if (size > mCapacity)
					setCapacity(size);

				if (size > mSize)
					new(mData + mSize) T[size - mSize];
			}
			else
			{
				for (size_t i = size; i < mSize; i++)
					mData[i].~T();
			}

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

		void shrink()
		{
			setCapacity(mSize);
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
		static T* allocateMemory(size_t capacity)
		{
			return reinterpret_cast<T*>(operator new(capacity * sizeof(T)));
		}

		static void freeMemory(T* data)
		{
			operator delete((void*)data);
		}

		size_t calculateNewCapacity()
		{
			return static_cast<size_t>(static_cast<double>(mCapacity) * CAPACITY_MULTIPLIER_FACTOR + 1.0);
		}

		size_t mSize;
		size_t mCapacity;
		T* mData;

		mutable size_t mCursor = 0;
	};

} // namespace Sonic
