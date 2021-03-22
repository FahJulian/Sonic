#pragma once

#include <optional>

namespace sonic 
{
	struct Nullopt
	{
	};

	template<typename T>
	class Optional
	{
	public:
		constexpr Optional()
			: mHasValue(false), _()
		{
		}

		constexpr Optional(Nullopt _)
			: mHasValue(false), _()
		{
		}

		Optional(const T& value)
			: mHasValue(true), mValue(value)
		{
		}

		~Optional()
		{
			if (mHasValue)
				mValue.~T();
		}

		operator bool() const
		{
			return mHasValue;
		}

		T* operator->()
		{
			return &mValue;
		}

		const T* operator->() const
		{
			return &mValue;
		}

		bool hasValue() const
		{
			return mHasValue;
		}

		T& getValue()
		{
			return mValue;
		}

		const T& getValue() const
		{
			return mValue;
		}

	private:
		bool mHasValue;

		union
		{
			T mValue;
			char _;
		};
	};

	constexpr Nullopt NULLOPT = { };

} // namespace sonic
