#include "String.h"

#include <cmath>
#include <istream>
#include <charconv>

#include "sonic/Base.h"
#include "sonic/util/StaticArray.h"

namespace sonic
{
	namespace
	{
		StaticArray<char, 8 * sizeof(long long)> STRING_CAST_BUFFER;

		template<typename T>
		String _floatToString(T value, bool scientific)
		{
			auto result = std::to_chars(STRING_CAST_BUFFER.begin(), STRING_CAST_BUFFER.end(), 
				value, scientific ? std::chars_format::scientific : std::chars_format::fixed, 2);

			return String(STRING_CAST_BUFFER.begin(), result.ptr - STRING_CAST_BUFFER.begin());
		}

		template<typename T>
		String _integerToString(T value, int base)
		{
			auto result = std::to_chars(STRING_CAST_BUFFER.begin(), STRING_CAST_BUFFER.end(), value, base);
			return String(STRING_CAST_BUFFER.begin(), result.ptr - STRING_CAST_BUFFER.begin());
		}

		char _toLowerCase(char c)
		{
			return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
		}

		char _toUpperCase(char c)
		{
			return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
		}

		bool _equalsIgnoreCase(const char* c1, const char* c2, size_t len)
		{
			for (size_t i = 0; i < len; i++)
			{
				if (_toLowerCase(c1[i]) != _toLowerCase(c2[i]))
					return false;
			}

			return true;
		}

		bool _isWhitespace(char c)
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

	bool String::startsWith(char c) const
	{
		if (isEmpty())
			return false;

		return mData[0] == c;
	}

	bool String::startsWith(const char* string) const
	{
		size_t len = strlen(string);

		if (len > mSize)
			return false;

		return std::strncmp(mData, string, len) == 0;
	}

	bool String::startsWith(const String& string) const
	{
		if (string.mSize > mSize)
			return false;

		return std::strncmp(mData, string, string.mSize) == 0;
	}

	bool String::startsWithIgnoreCase(char c) const
	{
		if (isEmpty())
			return false;

		return _toLowerCase(mData[0]) == _toLowerCase(c);
	}

	bool String::startsWithIgnoreCase(const char* string) const
	{
		size_t len = strlen(string);

		if (len > mSize)
			return false;

		return _equalsIgnoreCase(mData, string, len);
	}

	bool String::startsWithIgnoreCase(const String& string) const
	{
		if (string.mSize > mSize)
			return false;

		return _equalsIgnoreCase(mData, string, string.mSize);
	}

	bool String::endsWith(char c) const
	{
		if (isEmpty())
			return false;

		return mData[mSize - 1] == c;
	}

	bool String::endsWith(const char* string) const
	{
		size_t len = strlen(string);

		if (len > mSize)
			return false;

		return std::strncmp(mData + mSize - len, string, len) == 0;
	}

	bool String::endsWith(const String& string) const
	{
		if (string.mSize > mSize)
			return false;

		return std::strncmp(mData + mSize - string.mSize, string, string.mSize) == 0;
	}

	bool String::endsWithIgnoreCase(char c) const
	{
		if (isEmpty())
			return false;

		return _toLowerCase(mData[mSize - 1]) == _toLowerCase(c);
	}

	bool String::endsWithIgnoreCase(const char* string) const
	{
		size_t len = strlen(string);

		if (len > mSize)
			return false;

		return _equalsIgnoreCase(mData + mSize - len, string, len);
	}

	bool String::endsWithIgnoreCase(const String& string) const
	{
		if (string.mSize > mSize)
			return false;

		return _equalsIgnoreCase(mData + mSize - string.mSize, string, string.mSize);
	}

	bool String::contains(char c) const
	{
		return findFirstOf(c) != mSize;
	}

	bool String::contains(const String& string) const
	{
		return findFirstOf(string) != mSize;
	}

	bool String::contains(const char* string) const
	{
		return findFirstOf(string) != mSize;
	}

	bool String::containsIgnoreCase(char c) const
	{
		return findFirstOfIgnoreCase(c) != mSize;
	}

	bool String::containsIgnoreCase(const String& string) const
	{
		return findFirstOfIgnoreCase(string) != mSize;
	}

	bool String::containsIgnoreCase(const char* string) const
	{
		return findFirstOfIgnoreCase(string) != mSize;
	}

	bool String::replaceAll(char oldChar, char newChar)
	{
		bool atLeastOneReplacement = false;

		size_t index = 0;
		while ((index = replaceFirst(oldChar, newChar, index)) != mSize)
			atLeastOneReplacement = true;

		return atLeastOneReplacement;
	}

	bool String::replaceAll(const char* oldString, const char* newString)
	{
		SN_ASSERT(strlen(oldString) == strlen(newString), "replaceAll can only replace segments of equal size");
		bool atLeastOneReplacement = false;

		size_t index = 0;
		while ((index = replaceFirst(oldString, newString, index)) != mSize)
			atLeastOneReplacement = true;

		return atLeastOneReplacement;
	}

	bool String::replaceAll(const String& oldString, const String& newString)
	{
		SN_ASSERT(oldString.mSize == newString.mSize, "replaceAll can only replace segments of equal size");
		bool atLeastOneReplacement = false;

		size_t index = 0;
		while ((index = replaceFirst(oldString, newString, index)) != mSize)
			atLeastOneReplacement = true;

		return atLeastOneReplacement;
	}

	bool String::replaceAllIgnoreCase(char oldChar, char newChar)
	{
		bool atLeastOneReplacement = false;

		size_t index = 0;
		while ((index = replaceFirstIgnoreCase(oldChar, newChar, index)) != mSize)
			atLeastOneReplacement = true;

		return atLeastOneReplacement;
	}

	bool String::replaceAllIgnoreCase(const char* oldString, const char* newString)
	{
		SN_ASSERT(strlen(oldString) == strlen(newString), "replaceAllIgnoreCase can only replace segments of equal size");
		bool atLeastOneReplacement = false;

		size_t index = 0;
		while ((index = replaceFirstIgnoreCase(oldString, newString, index)) != mSize)
			atLeastOneReplacement = true;

		return atLeastOneReplacement;
	}

	bool String::replaceAllIgnoreCase(const String& oldString, const String& newString)
	{
		SN_ASSERT(oldString.mSize == newString.mSize, "replaceAllIgnoreCase can only replace segments of equal size");
		bool atLeastOneReplacement = false;

		size_t index = 0;
		while ((index = replaceFirstIgnoreCase(oldString, newString, index)) != mSize)
			atLeastOneReplacement = true;

		return atLeastOneReplacement;
	}

	size_t String::replaceFirst(char oldChar, char newChar, size_t startIndex)
	{
		size_t index = findFirstOf(oldChar, startIndex);

		if (index != mSize)
			mData[index] = newChar;

		return index;
	}

	size_t String::replaceFirst(const char* oldString, const char* newString, size_t startIndex)
	{
		SN_ASSERT(strlen(oldString) == strlen(newString), "replaceFirst can only replace segments of equal size");

		size_t index = findFirstOf(oldString, startIndex);

		if (index != mSize)
			std::memcpy(mData + index, newString, strlen(newString));

		return index;
	}

	size_t String::replaceFirst(const String& oldString, const String& newString, size_t startIndex)
	{
		SN_ASSERT(oldString.mSize == newString.mSize, "replaceFirst can only replace segments of equal size");

		size_t index = findFirstOf(oldString, startIndex);

		if (index != mSize)
			std::memcpy(mData + index, newString, newString.mSize);

		return index;
	}

	size_t String::replaceFirstIgnoreCase(char oldChar, char newChar, size_t startIndex)
	{
		size_t index = findFirstOfIgnoreCase(oldChar, startIndex);

		if (index != mSize)
			mData[index] = newChar;

		return index;
	}

	size_t String::replaceFirstIgnoreCase(const char* oldString, const char* newString, size_t startIndex)
	{
		SN_ASSERT(strlen(oldString) == strlen(newString), "replaceFirstIgnoreCase can only replace segments of equal size");

		size_t index = findFirstOfIgnoreCase(oldString, startIndex);

		if (index != mSize)
			std::memcpy(mData + index, newString, strlen(newString));

		return index;
	}

	size_t String::replaceFirstIgnoreCase(const String& oldString, const String& newString, size_t startIndex)
	{
		SN_ASSERT(oldString.mSize == newString.mSize, "replaceFirstIgnoreCase can only replace segments of equal size");

		size_t index = findFirstOfIgnoreCase(oldString, startIndex);

		if (index != mSize)
			std::memcpy(mData + index, newString, newString.mSize);

		return index;
	}

	size_t String::replaceLast(char oldChar, char newChar)
	{
		size_t index = findLastOf(oldChar);

		if (index != mSize)
			mData[index] = newChar;

		return index;
	}

	size_t String::replaceLast(const char* oldString, const char* newString)
	{
		SN_ASSERT(strlen(oldString) == strlen(newString), "replaceLast can only replace segments of equal size");

		size_t index = findLastOf(oldString);

		if (index != mSize)
			std::memcpy(mData + index, newString, strlen(newString));

		return index;
	}

	size_t String::replaceLast(const String& oldString, const String& newString)
	{
		SN_ASSERT(oldString.mSize == newString.mSize, "replaceLast can only replace segments of equal size");

		size_t index = findLastOf(oldString);

		if (index != mSize)
			std::memcpy(mData + index, newString, newString.mSize);

		return index;
	}

	size_t String::replaceLastIgnoreCase(char oldChar, char newChar)
	{
		size_t index = findLastOfIgnoreCase(oldChar);

		if (index != mSize)
			mData[index] = newChar;

		return index;
	}

	size_t String::replaceLastIgnoreCase(const char* oldString, const char* newString)
	{
		SN_ASSERT(strlen(oldString) == strlen(newString), "replaceLastIgnoreCase can only replace segments of equal size");

		size_t index = findLastOfIgnoreCase(oldString);

		if (index != mSize)
			std::memcpy(mData + index, newString, strlen(newString));

		return index;
	}

	size_t String::replaceLastIgnoreCase(const String& oldString, const String& newString)
	{
		SN_ASSERT(oldString.mSize == newString.mSize, "replaceLastIgnoreCase can only replace segments of equal size");

		size_t index = findLastOfIgnoreCase(oldString);

		if (index != mSize)
			std::memcpy(mData + index, newString, newString.mSize);

		return index;
	}

	size_t String::findFirstOf(char c, size_t startIndex) const
	{
		for (size_t i = startIndex; i < mSize; i++)
		{
			if (mData[i] == c)
				return i;
		}

		return mSize;
	}

	size_t String::findFirstOf(const char* string, size_t startIndex) const
	{
		size_t len = strlen(string);

		for (size_t i = startIndex; i <= mSize - len; i++)
		{
			if (strncmp(mData + i, string, len) == 0)
				return i;
		}

		return mSize;
	}

	size_t String::findFirstOf(const String& string, size_t startIndex) const
	{
		for (size_t i = startIndex; i <= mSize - string.mSize; i++)
		{
			if (strncmp(mData + i, string, string.mSize) == 0)
				return i;
		}

		return mSize;
	}

	size_t String::findFirstOfIgnoreCase(char c, size_t startIndex) const
	{
		c = _toLowerCase(c);

		for (size_t i = startIndex; i < mSize; i++)
		{
			if (_toLowerCase(mData[i]) == c)
				return i;
		}

		return mSize;
	}

	size_t String::findFirstOfIgnoreCase(const char* string, size_t startIndex) const
	{
		size_t len = strlen(string);

		for (size_t i = startIndex; i <= mSize - len; i++)
		{
			if (_equalsIgnoreCase(mData + i, string, len))
				return i;
		}

		return mSize;
	}

	size_t String::findFirstOfIgnoreCase(const String& string, size_t startIndex) const
	{
		for (size_t i = startIndex; i <= mSize - string.mSize; i++)
		{
			if (_equalsIgnoreCase(mData + i, string, string.mSize))
				return i;
		}

		return mSize;
	}

	size_t String::findLastOf(char c) const
	{
		for (size_t i = mSize; i > 0; i--)
		{
			if (mData[i - 1] == c)
				return i - 1;
		}

		return mSize;
	}

	size_t String::findLastOf(const char* string) const
	{
		size_t len = strlen(string);

		for (size_t i = mSize - len + 1; i > 0; i--)
		{
			if (strncmp(mData + i - 1, string, len) == 0)
				return i - 1;
		}

		return mSize;
	}

	size_t String::findLastOf(const String& string) const
	{
		for (size_t i = mSize - string.mSize + 1; i > 0; i--)
		{
			if (strncmp(mData + i - 1, string, string.mSize) == 0)
				return i - 1;
		}

		return mSize;
	}

	size_t String::findLastOfIgnoreCase(char c) const
	{
		c = _toLowerCase(c);

		for (size_t i = mSize; i > 0; i--)
		{
			if (_toLowerCase(mData[i - 1]) == c)
				return i - 1;
		}

		return mSize;
	}

	size_t String::findLastOfIgnoreCase(const char* string) const
	{
		size_t len = strlen(string);

		for (size_t i = mSize - len + 1; i > 0; i--)
		{
			if (_equalsIgnoreCase(mData + i - 1, string, len))
				return i - 1;
		}

		return mSize;
	}

	size_t String::findLastOfIgnoreCase(const String& string) const
	{
		for (size_t i = mSize - string.mSize + 1; i > 0; i--)
		{
			if (_equalsIgnoreCase(mData + i - 1, string, string.mSize))
				return i - 1;
		}

		return mSize;
	}

	size_t String::findFirstNotOf(char c, size_t startIndex) const 
	{
		for (size_t i = startIndex; i < mSize; i++)
		{
			if (mData[i] != c)
				return i;
		}

		return mSize;
	}

	size_t String::findFirstNotOf(const char* string, size_t startIndex) const
	{
		size_t len = strlen(string);

		for (size_t i = startIndex; i <= mSize - len; i++)
		{
			if (strncmp(mData + i, string, len) != 0)
				return i;
		}

		return mSize;
	}	

	size_t String::findFirstNotOf(const String& string, size_t startIndex) const
	{
		for (size_t i = startIndex; i <= mSize - string.mSize; i++)
		{
			if (strncmp(mData + i, string, string.mSize) != 0)
				return i;
		}

		return mSize;
	}

	size_t String::findFirstNotOfIgnoreCase(char c, size_t startIndex) const
	{
		c = _toLowerCase(c);

		for (size_t i = startIndex; i < mSize; i++)
		{
			if (_toLowerCase(mData[i]) != c)
				return i;
		}

		return mSize;
	}

	size_t String::findFirstNotOfIgnoreCase(const char* string, size_t startIndex) const
	{
		size_t len = strlen(string);

		for (size_t i = startIndex; i <= mSize - len; i++)
		{
			if (!_equalsIgnoreCase(mData + i, string, len))
				return i;
		}

		return mSize;
	}

	size_t String::findFirstNotOfIgnoreCase(const String& string, size_t startIndex) const
	{
		for (size_t i = startIndex; i <= mSize - string.mSize; i++)
		{
			if (!_equalsIgnoreCase(mData + i, string, string.mSize))
				return i;
		}

		return mSize;
	}

	size_t String::findLastNotOf(char c) const
	{
		for (size_t i = mSize; i > 0; i--)
		{
			if (mData[i - 1] != c)
				return i - 1;
		}

		return mSize;
	}

	size_t String::findLastNotOf(const char* string) const
	{
		size_t len = strlen(string);

		for (size_t i = mSize - len + 1; i > 0; i--)
		{
			if (strncmp(mData + i - 1, string, len) != 0)
				return i - 1;
		}

		return mSize;
	}

	size_t String::findLastNotOf(const String& string) const
	{
		for (size_t i = mSize - string.mSize + 1; i > 0; i--)
		{
			if (strncmp(mData + i - 1, string, string.mSize) != 0)
				return i - 1;
		}

		return mSize;
	}

	size_t String::findLastNotOfIgnoreCase(char c) const
	{
		c = _toLowerCase(c);

		for (size_t i = mSize; i > 0; i--)
		{
			if (_toLowerCase(mData[i - 1]) != c)
				return i - 1;
		}

		return mSize;
	}

	size_t String::findLastNotOfIgnoreCase(const char* string) const
	{
		size_t len = strlen(string);

		for (size_t i = mSize - len + 1; i > 0; i--)
		{
			if (!_equalsIgnoreCase(mData + i - 1, string, len))
				return i - 1;
		}

		return mSize;
	}

	size_t String::findLastNotOfIgnoreCase(const String& string) const
	{
		for (size_t i = mSize - string.mSize + 1; i > 0; i--)
		{
			if (!_equalsIgnoreCase(mData + i - 1, string, string.mSize))
				return i - 1;
		}

		return mSize;
	}

	DynamicArray<String> String::split(char c) const
	{
		if (isEmpty())
			return { };

		DynamicArray<String> parts;

		const char* nextPartStart = mData;
		for (const char* i = mData; i < mData + mSize; i++)
		{
			if (*i == c)
			{
				if (size_t length = i - nextPartStart;
					length > 0)
				{
					parts.add(nextPartStart, length);
				}

				nextPartStart = i + 1;
			}
		}

		if (size_t length = mData + mSize - nextPartStart;
			length > 0)
		{
			parts.add(nextPartStart, length);
		}

		return parts;
	}

	DynamicArray<String> String::split(const char* string) const
	{
		size_t len = strlen(string);

		if (len >= mSize)
			return { };

		DynamicArray<String> parts;

		const char* i = mData;
		const char* nextPartStart = mData;
		while (i <= mData + mSize - len)
		{
			if (std::strncmp(string, i, len) == 0)
			{
				if (size_t length = i - nextPartStart;
					length > 0)
				{
					parts.add(nextPartStart, length);
				}

				nextPartStart = i + len;
				i += len;
			}
			else
			{
				i++;
			}
		}

		if (size_t length = mData + mSize - nextPartStart;
			length > 0)
		{
			parts.add(nextPartStart, length);
		}

		return parts;
	}

	DynamicArray<String> String::split(const String& string) const
	{
		if (string.mSize >= mSize)
			return { };

		DynamicArray<String> parts;

		const char* i = mData;
		const char* nextPartStart = mData;
		while (i <= mData + mSize - string.mSize)
		{
			if (std::strncmp(string, i, string.mSize) == 0)
			{
				if (size_t length = i - nextPartStart;
					length > 0)
				{
					parts.add(nextPartStart, length);
				}

				nextPartStart = i + string.mSize;
				i += string.mSize;
			}
			else
			{
				i++;
			}
		}

		if (size_t length = mData + mSize - nextPartStart;
			length > 0)
		{
			parts.add(nextPartStart, length);
		}

		return parts;
	}

	DynamicArray<String> String::splitIgnoreCase(char c) const
	{
		if (isEmpty())
			return { };

		DynamicArray<String> parts;

		c = _toLowerCase(c);

		const char* nextPartStart = mData;
		for (const char* i = mData; i < mData + mSize; i++)
		{
			if (_toLowerCase(*i) == c)
			{
				if (size_t length = i - nextPartStart;
					length > 0)
				{
					parts.add(nextPartStart, length);
				}

				nextPartStart = i + 1;
			}
		}

		if (size_t length = mData + mSize - nextPartStart;
			length > 0)
		{
			parts.add(nextPartStart, length);
		}

		return parts;
	}

	DynamicArray<String> String::splitIgnoreCase(const char* string) const
	{
		size_t len = strlen(string);

		if (len >= mSize)
			return { };

		DynamicArray<String> parts;

		const char* i = mData;
		const char* nextPartStart = mData;
		while (i <= mData + mSize - len)
		{
			if (_equalsIgnoreCase(string, i, len))
			{
				if (size_t length = i - nextPartStart;
					length > 0)
				{
					parts.add(nextPartStart, length);
				}

				nextPartStart = i + len;
				i += len;
			}
			else
			{
				i++;
			}
		}

		if (size_t length = mData + mSize - nextPartStart;
			length > 0)
		{
			parts.add(nextPartStart, length);
		}

		return parts;
	}

	DynamicArray<String> String::splitIgnoreCase(const String& string) const
	{
		if (string.mSize >= mSize)
			return { };

		DynamicArray<String> parts;

		const char* i = mData;
		const char* nextPartStart = mData;
		while (i <= mData + mSize - string.mSize)
		{
			if (_equalsIgnoreCase(string, i, string.mSize))
			{
				if (size_t length = i - nextPartStart;
					length > 0)
				{
					parts.add(nextPartStart, length);
				}

				nextPartStart = i + string.mSize;
				i += string.mSize;
			}
			else
			{
				i++;
			}
		}

		if (size_t length = mData + mSize - nextPartStart;
			length > 0)
		{
			parts.add(nextPartStart, length);
		}

		return parts;
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

		for (size_t i = mSize; i > 0; i--)
		{
			if (!_isWhitespace(mData[i - 1]))
			{
				LastNotWhiteSpace = i - 1;
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

	const char& String::get(size_t index) const
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

	bool operator<<(String& string, std::istream& file)
	{
		size_t posBefore = file.tellg();

		file.seekg(0, file.end);
		size_t fileSize = file.tellg();

		if (fileSize == -1)
			return false;

		file.seekg(0, file.beg);
		string.setSize(fileSize);
		file.read(string.getData(), fileSize);

		file.seekg(posBefore, file.beg);

		return string;
	}

	bool operator<<(String& string, std::istream&& file)
	{
		size_t posBefore = file.tellg();

		file.seekg(0, file.end);
		size_t fileSize = file.tellg();

		if (fileSize == -1)
			return false;

		file.seekg(0, file.beg);
		string.setSize(fileSize);
		file.read(string.getData(), fileSize);

		file.seekg(posBefore, file.beg);

		return true;
	}

	String operator+(const char* string1, const String& string2)
	{
		size_t len = strlen(string1);
		String string = String(len + string2.getSize());

		std::memcpy(string.getData(), string1, len);
		std::memcpy(string.getData() + len, string2.getData(), string2.getSize());

		return string;
	}

	String operator+(const String& string1, const char* string2)
	{
		size_t len = strlen(string2);
		String string = String(string1.getSize() + len);

		std::memcpy(string.getData(), string1.getData(), string1.getSize());
		std::memcpy(string.getData() + string1.getSize(), string2, len);

		return string;
	}

} // namespace sonic
