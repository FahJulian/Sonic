#pragma once

#include "DynamicArray.h"

namespace sonic {

	template<typename K, typename V>
	class Map
	{
	public:
		struct Pair
		{
			Pair() = default;

			Pair(const K& key, const V& value)
				: key(key), value(value)
			{
			}

			template<typename T, typename F>
			Pair(const T& key, const F& value)
				: key(key), value(value)
			{
			}

			template<typename T, typename F>
			Pair(typename Map<T, F>::Pair&& other)
			{
				*this = std::move(other);
			}

			template<typename T, typename F>
			Pair(typename const Map<T, F>::Pair& other)
			{
				*this = other;
			}

			template<typename T>
			Pair& operator=(T&& otherValue)
			{
				value = std::move(otherValue);

				return *this;
			}

			template<typename T, typename F>
			Pair& operator=(typename const Map<T, F>::Pair& other)
			{
				key = other.key;
				value = other.value;

				return *this;
			}

			template<typename T, typename F>
			Pair& operator=(typename Map<T, F>::Pair&& other)
			{
				key = std::move(other.key);
				value = std::move(other.value);

				return *this;
			}

			template<typename T, typename F>
			bool operator==(typename const Map<T, F>::Pair& other) const
			{
				return key == other.key;
			}

			template<typename T>
			bool operator==(const T& otherKey) const
			{
				return key == otherKey;
			}

			K key;
			V value;
		};

		constexpr Map() = default;

		Map(size_t capacity)
			: mData(capacity)
		{
		}

		Map(const std::initializer_list<Pair> values)
			: mData(values)
		{
		}

		Map(const Map& other)
			: mData(other.mData)
		{
		}

		Map(Map&& other) noexcept
			: mData(std::move(other.mData))
		{
		}

		~Map() = default;

		Map& operator=(const Map& other)
		{
			mData = other.mData;

			return *this;
		}

		Map& operator=(Map&& other) noexcept
		{
			mData = std::move(other.mData);

			return *this;
		}

		template<typename T>
		V& operator[](const T& key)
		{
			return mData.get(mData.indexOf(key)).value;
		}

		template<typename T>
		const V& operator[](const T& key) const
		{
			return mData.get(mData.indexOf(key)).value;
		}

		template<typename T, typename F>
		void insert(const T& key, const F& value)
		{
			if (mData.contains(key))
				mData.get(mData.indexOf(key)).value = value;
			else
				mData.add(key, value);
		}

		void insert(const K& key, const V& value)
		{
			if (mData.contains(key))
				mData.get(mData.indexOf(key)).value = value;
			else
				mData.add(key, value);
		}

		void insert(const Pair& pair)
		{
			if (mData.contains(pair.key))
				mData.get(mData.indexOf(pair.key)).value = pair.value;
			else
				mData.add(pair.key, pair.value);
		}

		template<typename T>
		bool containsKey(const T& key) const
		{
			return mData.contains(key);
		}

		template<typename T>
		bool containsValue(const T& value) const
		{
			for (auto& [k, v] : *this)
			{
				if (v == value)
					return true;
			}

			return false;
		}

		template<typename T>
		void remove(const T& key)
		{
			mData.remove(mData.indexOf(key));
		}

		template<typename F>
		void forEach(F function)
		{
			mData.forEach(function);
		}

		void setSize(size_t size)
		{
			mData.setSize(size);
		}

		void setCapacity(size_t capacity)
		{
			mData.setSize(capacity);
		}

		void shrink()
		{
			mData.shrink();
		}

		void clear()
		{
			mData.clear();
		}

		Pair* begin()
		{
			return mData.begin();
		}

		Pair* end()
		{
			return mData.end();
		}

		const Pair* begin() const
		{
			return mData.begin();
		}

		const Pair* end() const
		{
			return mData.end();
		}

		Pair* getData()
		{
			return mData.getData();
		}

		const Pair* getData() const
		{
			return mData.getData();
		}

		size_t getSize() const
		{
			return mData.getSize();
		}

		size_t getCapacity() const
		{
			return mData.getCapacity();
		}

		template<typename T>
		V& get(const T& key)
		{
			return mData.get(mData.indexOf(key)).value;
		}

		template<typename T>
		const V& get(const T& key) const
		{
			return mData.get(mData.indexOf(key)).value;
		}

	private:
		DynamicArray<Pair> mData;
	};

} // namespace sonic
