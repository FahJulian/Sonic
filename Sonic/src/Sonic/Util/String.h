#pragma once

#include <ostream>

namespace sonic
{
	class String
	{
	public:
		constexpr String()
			: mSize(0), mData(nullptr)
		{
		}

		String(const char* data);

		String(const char* data, size_t size);

		String(const String& other);

		String(String&& other) noexcept;

		~String();

		String& operator=(const String& other);

		String& operator=(String&& other) noexcept;

		char& operator[](size_t index);

		char* begin()
		{
			return mData;
		}

		char* end()
		{
			return mData + mSize;
		}

		const char* begin() const
		{
			return mData;
		}

		const char* end() const
		{
			return mData + mSize;
		}

		size_t getSize() const
		{
			return mSize;
		}

		char* getData()
		{
			return mData;
		}

		const char* getData() const
		{
			return mData;
		}

	private:
		size_t mSize;
		char* mData;
	};

	std::ostream& operator<<(std::ostream& stream, const String& string);

} // namespace Sonic
