#include "String.h"

#include <cmath>
#include <charconv>

#include "sonic/Base.h"
#include "sonic/util/StaticArray.h"

namespace sonic
{
	namespace
	{
		static StaticArray<char, 8 * sizeof(long long)> STRING_CAST_BUFFER;

		template<typename T>
		static String _floatToString(T value, bool scientific)
		{
			auto result = std::to_chars(STRING_CAST_BUFFER.begin(), STRING_CAST_BUFFER.end(), 
				value, scientific ? std::chars_format::scientific : std::chars_format::fixed, 2);

			return String(STRING_CAST_BUFFER.begin(), result.ptr - STRING_CAST_BUFFER.begin());
		}

		template<typename T>
		static String _integerToString(T value, int base)
		{
			auto result = std::to_chars(STRING_CAST_BUFFER.begin(), STRING_CAST_BUFFER.end(), value, base);
			return String(STRING_CAST_BUFFER.begin(), result.ptr - STRING_CAST_BUFFER.begin());
		}

		static char _toLowerCase(char c)
		{
			return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
		}

		static char _toUpperCase(char c)
		{
			return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
		}

		static bool _isWhitespace(char c)
		{
			return c <= ' ';
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

	bool String::equals(const String& other) const
	{
		return std::strcmp(mData, other) == 0;
	}

	bool String::equals(const char* other) const
	{
		return std::strcmp(mData, other) == 0;
	}

	bool String::equalsIgnoreCase(const String& other) const
	{
		if (mSize != other.mSize)
			return false;

		for (size_t i = 0; i < mSize; i++)
		{
			if (_toLowerCase(mData[i]) != _toLowerCase(other.mData[i]))
				return false;
		}

		return true;
	}

	bool String::equalsIgnoreCase(const char* other) const
	{
		if (mSize != strlen(other))
			return false;

		for (size_t i = 0; i < mSize; i++)
		{
			if (_toLowerCase(mData[i]) != _toLowerCase(other[i]))
				return false;
		}

		return true;
	}

	bool String::contains(char c)
	{
		return false;
	}

	bool String::contains(const String& other) const
	{
		return false;
	}

	bool String::contains(const char* other) const
	{
		return false;
	}

	bool String::containsIgnoreCase(char c)
	{
		return false;
	}

	bool String::containsIgnoreCase(const String& string) const
	{
		return false;
	}

	bool String::containsIgnoreCase(const char* string) const
	{
		return false;
	}

	bool String::endsWith(const String& string) const
	{
		return false;
	}

	bool String::endsWith(const char* string) const
	{
	return false;
	}

	bool String::startsWith(const String& string) const
	{
		return false;
	}

	bool String::startsWith(const char* string) const
	{
		return false;
	}

	bool String::replaceAll(char oldChar, char newChar)
	{
		return false;
	}

	bool String::replaceAll(const char* oldString, const char* newString)
	{
		return false;
	}

	bool String::replaceAll(const String& oldString, const String& newString)
	{
		return false;
	}

	bool String::replaceFirst(char firstChar, char newChar)
	{
		return false;
	}

	bool String::replaceFirst(const char* oldString, const char* newString)
	{
		return false;
	}

	bool String::replaceFirst(const String& oldString, const String& newString)
	{
		return false;
	}

	bool String::replaceLast(char firstChar, char newChar)
	{
		return false;
	}

	bool String::replaceLast(const char* oldString, const char* newString)
	{
		return false;
	}

	bool String::replaceLast(const String& oldString, const String& newString)
	{
		return false;
	}

	size_t String::findFirst(char c) const
	{
		return 0;
	}

	size_t String::findFirst(const char* string) const
	{
		return 0;
	}

	size_t String::findFirst(const String& string) const
	{
		return 0;
	}

	size_t String::findLast(char c) const
	{
		return 0;
	}

	size_t String::findLast(const char* string) const
	{
		return 0;
	}

	size_t String::findLast(const String& string) const
	{
		return 0;
	}

	DynamicArray<String> String::split(char c) const
	{
		return { };
	}

	DynamicArray<String> String::split(const char* string) const
	{
		return { };
	}

	DynamicArray<String> String::split(const String& string) const
	{
		return { };
	}

	String String::subString(size_t beginIndex) const
	{
		SN_ASSERT(beginIndex <= mSize, "beginIndex must be less than size");

		return String(mData + beginIndex, mSize - beginIndex);
	}

	String String::subString(size_t beginIndex, size_t endIndex) const
	{
		SN_ASSERT(beginIndex <= mSize, "beginIndex must be less than size");
		SN_ASSERT(endIndex <= mSize, "endIndex must be less than size");
		SN_ASSERT(endIndex >= beginIndex, "endIndex must be >= beginIndex");

		return String(mData + beginIndex, endIndex - beginIndex);
	}

	String String::trim() const
	{
		size_t firstNotWhiteSpace = 0;
		size_t LastNotWhiteSpace = 0;

		for (size_t i = 0; i < mSize; i++)
		{
			if (!_isWhitespace(mData[i]))
			{
				firstNotWhiteSpace = i;
				break;
			}
		}

		for (size_t i = mSize - 1; i >= 0; i--)
		{
			if (!_isWhitespace(mData[i]))
			{
				LastNotWhiteSpace = i;
				break;
			}
		}

		return subString(firstNotWhiteSpace, LastNotWhiteSpace + 1);
	}

	String String::toLowerCase() const
	{
		String string = String(mSize);

		for (size_t i = 0; i < mSize; i++)
			string[i] = _toLowerCase(mData[i]);

		return string;
	}

	String String::toUpperCase() const
	{
		String string = String(mSize);

		for (size_t i = 0; i < mSize; i++)
			string[i] = _toUpperCase(mData[i]);

		return string;
	}

	char& String::get(size_t index)
	{
		SN_ASSERT(index < mSize, "Index out of range");

		return mData[index];
	}

	String String::valueOf(bool value)
	{
		return value ? String("true", 4) : String("false", 5);
	}

	String String::valueOf(char value, int base)
	{
		return _integerToString<char>(value, base);
	}

	String String::valueOf(unsigned char value, int base)
	{
		return _integerToString<unsigned char>(value, base);
	}

	String String::valueOf(short value, int base)
	{
		return _integerToString<short>(value, base);
	}

	String String::valueOf(unsigned short value, int base)
	{
		return _integerToString<unsigned short>(value, base);
	}

	String String::valueOf(int value, int base)
	{
		return _integerToString<int>(value, base);
	}

	String String::valueOf(unsigned int value, int base)
	{
		return _integerToString<unsigned int>(value, base);
	}

	String String::valueOf(long value, int base)
	{
		return _integerToString<long>(value, base);
	}

	String String::valueOf(unsigned long value, int base)
	{
		return _integerToString<unsigned long>(value, base);
	}

	String String::valueOf(long long value, int base)
	{
		return _integerToString<long long>(value, base);
	}

	String String::valueOf(unsigned long long value, int base)
	{
		return _integerToString<unsigned long long>(value, base);
	}

	String String::valueOf(float value, bool scientific)
	{
		return _floatToString<float>(value, scientific);
	}

	String String::valueOf(double value, bool scientific)
	{
		return _floatToString<double>(value, scientific);
	}

	String String::valueOf(long double value, bool scientific)
	{
		return _floatToString<long double>(value, scientific);
	}

	std::ostream& operator<<(std::ostream& stream, const String& string)
	{
		stream << string.getData();
		return stream;
	}

} // namespace sonic
