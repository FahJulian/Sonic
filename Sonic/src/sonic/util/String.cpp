#include "String.h"

#include <cmath>
#include <charconv>

#include "sonic/Base.h"

namespace sonic
{
	namespace
	{
		template<typename T>
		static String floatToString(T value, bool scientific)
		{
			String string;

			if (scientific)
			{
				string.setSize(8);
				std::to_chars(string.begin(), string.end(), value, std::chars_format::scientific, 2);
			}
			else
			{
				string.setSize(static_cast<size_t>(std::log10(value)) + 4);
				std::to_chars(string.begin(), string.end(), value, std::chars_format::fixed, 2);
			}

			return string;
		}

	} // namespace

	String::String(const char* data)
		: mSize(std::strlen(data)), mData(new char[mSize + 1])
	{
		std::memcpy(mData, data, mSize);
		mData[mSize] = '\0';
	}

	String::String(const char* data, size_t size)
		: mSize(size), mData(new char[mSize + 1])
	{
		if (data != nullptr)
			std::memcpy(mData, data, mSize);

		mData[mSize] = '\0';
	}

	String::String(size_t size)
		: String(nullptr, size)
	{
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

	String::~String()
	{
		delete[] mData;
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

	void String::append(const String& string)
	{
		char* newData = new char[mSize + string.mSize + 1];
		std::memcpy(newData, mData, mSize);
		std::memcpy(newData + mSize, string.mData, string.mSize);

		mSize += string.mSize;
		newData[mSize] = '\0';

		delete[] mData;
		mData = newData;
	}

	void String::append(const char* string)
	{
		size_t len = strlen(string);

		char* newData = new char[mSize + len + 1];
		std::memcpy(newData, mData, mSize);
		std::memcpy(newData + mSize, string, len);

		mSize += len;
		newData[mSize] = '\0';

		delete[] mData;
		mData = newData;
	}

	void String::insert(size_t index, const String& string)
	{
		char* newData = new char[mSize + string.mSize + 1];
		std::memcpy(newData, mData, index);
		std::memcpy(newData + index, string.mData, string.mSize);
		std::memcpy(newData + index + string.mSize, mData + index, mSize - index);

		mSize += string.mSize;
		newData[mSize] = '\0';

		delete[] mData;
		mData = newData;
	}

	void String::insert(size_t index, const char* string)
	{
		size_t len = strlen(string);

		char* newData = new char[mSize + len + 1];
		std::memcpy(newData, mData, index);
		std::memcpy(newData + index, string, len);
		std::memcpy(newData + index + len, mData + index, mSize - index);

		mSize += len;
		newData[mSize] = '\0';

		delete[] mData;
		mData = newData;
	}

	void String::setSize(size_t size)
	{
		char* newData = new char[size + 1];
		std::memcpy(newData, mData, mSize);

		mSize = size;
		newData[mSize] = '\0';

		delete[] mData;
		mData = newData;
	}

	char& String::get(size_t index)
	{
		SN_ASSERT(index < mSize, "Index ", index, " is out of range.");

		return mData[index];
	}

	String String::valueOf(bool value)
	{
		return value ? String("true", 4) : String("false", 5);
	}

	String String::valueOf(char value)
	{
		return String(&value, 1);
	}

	String String::valueOf(unsigned char value)
	{
		return String(reinterpret_cast<char*>(&value), 1);
	}

	String String::valueOf(short value)
	{
		return String();
	}

	String String::valueOf(unsigned short value)
	{
		return String();
	}

	String String::valueOf(int value)
	{
		return String();
	}

	String String::valueOf(unsigned int value)
	{
		return String();
	}

	String String::valueOf(long value)
	{
		return String();
	}

	String String::valueOf(unsigned long value)
	{
		return String();
	}

	String String::valueOf(long long value)
	{
		return String();
	}

	String String::valueOf(unsigned long long value)
	{
		return String();
	}

	String String::valueOf(float value, bool scientific)
	{
		return floatToString<float>(value, scientific);
	}

	String String::valueOf(double value, bool scientific)
	{
		return floatToString<double>(value, scientific);
	}

	String String::valueOf(long double value, bool scientific)
	{
		return floatToString<long double>(value, scientific);
	}

	std::ostream& operator<<(std::ostream& stream, const String& string)
	{
		stream << string.getData();
		return stream;
	}

} // namespace sonic
