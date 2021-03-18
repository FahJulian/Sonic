#include "String.h"

#include "sonic/Base.h"

namespace sonic
{
	String::String(const char* data)
		: mSize(std::strlen(data)), mData(new char[mSize + 1])
	{
		std::memcpy(mData, data, mSize);
		mData[mSize] = '\0';
	}

	String::String(const char* data, size_t size)
		: mSize(size), mData(new char[mSize + 1])
	{
		std::memcpy(mData, data, mSize);
		mData[mSize] = '\0';
	}

	String::String(const String& other)
		: mSize(other.mSize), mData(new char[mSize + 1])
	{
		std::memcpy(mData, other.mData, mSize);
		mData[mSize] = '\0';
	}

	String::String(String&& other) noexcept
		: mSize(other.mSize), mData(other.mData)
	{
		other.mSize = 0;
		other.mData = nullptr;
	}

	String& String::operator=(const String& other)
	{
		if (this != &other)
		{
			mSize = other.mSize;

			delete[] mData;
			mData = new char[mSize + 1];

			std::memcpy(mData, other.mData, mSize);
			mData[mSize] = '\0';
		}

		return *this;
	}

	String& String::operator=(String&& other) noexcept
	{
		if (this != &other)
		{
			mSize = other.mSize;

			delete[] mData;
			mData = other.mData;

			other.mSize = 0;
			other.mData = nullptr;
		}

		return *this;
	}

	String::~String()
	{
		delete[] mData;
	}

	char& String::operator[](size_t index)
	{
		SN_ASSERT(index < mSize, "Index ", index, " is out of range.");

		return mData[index];
	}

	std::ostream& operator<<(std::ostream& stream, const String& string)
	{
		stream << string.getData();
		return stream;
	}

} // namespace sonic
