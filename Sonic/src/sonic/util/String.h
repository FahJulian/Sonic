#pragma once

#include <ostream>
#include <limits>

#include "DynamicArray.h"

#if defined(max)
	#undef max
#endif

namespace sonic
{
	class String
	{
	public:
		constexpr String() noexcept
			: mSize(0), mData(nullptr)
		{
		}

		String(char c);

		String(const char* data);

		explicit String(const char* data, size_t size);

		explicit String(size_t size);

		String(const String& other);

		String(String&& other) noexcept;

		~String();

		String& operator=(char c);
		String& operator=(const char* other);
		String& operator=(const String& other);

		String& operator=(String&& other) noexcept;

		char& operator[](size_t index) { return get(index); }
		const char& operator[](size_t index) const { return get(index); }

		void operator+=(char c) { append(c); }
		void operator+=(const String& string) { append(string); }
		void operator+=(const char* string) { append(string); }

		bool operator==(char c) const { return equals(c); }
		bool operator==(const String& other) const { return equals(other); }
		bool operator==(const char* other) const { return equals(other); }

		bool operator!=(char c) const { return !equals(c); }
		bool operator!=(const String& other) const { return !equals(other); }
		bool operator!=(const char* other) const { return !equals(other); }

		String& append(char c);
		String& append(const String& string);
		String& append(const char* string);

		String& insert(size_t index, char c);
		String& insert(size_t index, const String& string);
		String& insert(size_t index, const char* string);

		bool equals(char c) const;
		bool equals(const String& other) const;
		bool equals(const char* other) const;

		bool equalsIgnoreCase(char c) const;
		bool equalsIgnoreCase(const String& other) const;
		bool equalsIgnoreCase(const char* other) const;

		bool startsWith(char c) const;
		bool startsWith(const char* string) const;
		bool startsWith(const String& string) const;

		bool startsWithIgnoreCase(char c) const;
		bool startsWithIgnoreCase(const char* string) const;
		bool startsWithIgnoreCase(const String& string) const;

		bool endsWith(char c) const;
		bool endsWith(const char* string) const;
		bool endsWith(const String& string) const;

		bool endsWithIgnoreCase(char c) const;
		bool endsWithIgnoreCase(const char* string) const;
		bool endsWithIgnoreCase(const String& string) const;

		bool contains(char c) const;
		bool contains(const String& string) const;
		bool contains(const char* string) const;

		bool containsIgnoreCase(char c) const;
		bool containsIgnoreCase(const String& string) const;
		bool containsIgnoreCase(const char* string) const;

		String& replace(size_t beginIndex, size_t endIndex, char c);
		String& replace(size_t beginIndex, size_t endIndex, const char* string);
		String& replace(size_t beginIndex, size_t endIndex, const String& string);

		String& replaceAll(char oldChar, char newChar, size_t startIndex = 0);
		String& replaceAll(const char* oldString, const char* newString, size_t startIndex = 0);
		String& replaceAll(const String& oldString, const String& newString, size_t startIndex = 0);

		String& replaceAllIgnoreCase(char oldChar, char newChar, size_t startIndex = 0);
		String& replaceAllIgnoreCase(const char* oldString, const char* newString, size_t startIndex = 0);
		String& replaceAllIgnoreCase(const String& oldString, const String& newString, size_t startIndex = 0);

		String& replaceFirst(char oldChar, char newChar, size_t startIndex = 0);
		String& replaceFirst(const char* oldString, const char* newString, size_t startIndex = 0);
		String& replaceFirst(const String& oldString, const String& newString, size_t startIndex = 0);

		String& replaceFirstIgnoreCase(char oldChar, char newChar, size_t startIndex = 0);
		String& replaceFirstIgnoreCase(const char* oldString, const char* newString, size_t startIndex = 0);
		String& replaceFirstIgnoreCase(const String& oldString, const String& newString, size_t startIndex = 0);

		String& replaceLast(char oldChar, char newChar, size_t endIndex = std::numeric_limits<size_t>::max());
		String& replaceLast(const char* oldString, const char* newString, size_t endIndex = std::numeric_limits<size_t>::max());
		String& replaceLast(const String& oldString, const String& newString, size_t endIndex = std::numeric_limits<size_t>::max());

		String& replaceLastIgnoreCase(char oldChar, char newChar, size_t endIndex = std::numeric_limits<size_t>::max());
		String& replaceLastIgnoreCase(const char* oldString, const char* newString, size_t endIndex = std::numeric_limits<size_t>::max());
		String& replaceLastIgnoreCase(const String& oldString, const String& newString, size_t endIndex = std::numeric_limits<size_t>::max());

		size_t findFirstOf(char c, size_t startIndex = 0) const;
		size_t findFirstOf(const char* string, size_t startIndex = 0) const;
		size_t findFirstOf(const String& string, size_t startIndex = 0) const;

		size_t findFirstOfIgnoreCase(char c, size_t startIndex = 0) const;
		size_t findFirstOfIgnoreCase(const char* string, size_t startIndex = 0) const;
		size_t findFirstOfIgnoreCase(const String& string, size_t startIndex = 0) const;

		size_t findLastOf(char c, size_t endIndex = std::numeric_limits<size_t>::max()) const;
		size_t findLastOf(const char* string, size_t endIndex = std::numeric_limits<size_t>::max()) const;
		size_t findLastOf(const String& string, size_t endIndex = std::numeric_limits<size_t>::max()) const;

		size_t findLastOfIgnoreCase(char c, size_t endIndex = std::numeric_limits<size_t>::max()) const;
		size_t findLastOfIgnoreCase(const char* string, size_t endIndex = std::numeric_limits<size_t>::max()) const;
		size_t findLastOfIgnoreCase(const String& string, size_t endIndex = std::numeric_limits<size_t>::max()) const;

		size_t findFirstNotOf(char c, size_t startIndex = 0) const;
		size_t findFirstNotOf(const char* string, size_t startIndex = 0) const;
		size_t findFirstNotOf(const String& string, size_t startIndex = 0) const;

		size_t findFirstNotOfIgnoreCase(char c, size_t startIndex = 0) const;
		size_t findFirstNotOfIgnoreCase(const char* string, size_t startIndex = 0) const;
		size_t findFirstNotOfIgnoreCase(const String& string, size_t startIndex = 0) const;

		size_t findLastNotOf(char c, size_t endIndex = std::numeric_limits<size_t>::max()) const;
		size_t findLastNotOf(const char* string, size_t endIndex = std::numeric_limits<size_t>::max()) const;
		size_t findLastNotOf(const String& string, size_t endIndex = std::numeric_limits<size_t>::max()) const;

		size_t findLastNotOfIgnoreCase(char c, size_t endIndex = std::numeric_limits<size_t>::max()) const;
		size_t findLastNotOfIgnoreCase(const char* string, size_t endIndex = std::numeric_limits<size_t>::max()) const;
		size_t findLastNotOfIgnoreCase(const String& string, size_t endIndex = std::numeric_limits<size_t>::max()) const;

		DynamicArray<String> split(char c) const;
		DynamicArray<String> split(const char* string) const;
		DynamicArray<String> split(const String& string) const;

		DynamicArray<String> splitIgnoreCase(char c) const;
		DynamicArray<String> splitIgnoreCase(const char* string) const;
		DynamicArray<String> splitIgnoreCase(const String& string) const;

		String subString(size_t beginIndex) const;
		String subString(size_t beginIndex, size_t endIndex) const;

		String trim() const;

		String toLowerCase() const;

		String toUpperCase() const;

		char* begin() { return mData; }
		const char* begin() const { return mData; }

		char* end() { return mData + mSize; }
		const char* end() const { return mData + mSize; }

		void setSize(size_t size);

		size_t getSize() const { return mSize; }

		bool isEmpty() const { return mSize == 0; }

		char& get(size_t index);
		const char& get(size_t index) const;

		char* getData() { return mData; }
		const char* getData() const { return mData; }

		static String valueOf(bool b);
		static String valueOf(char c, int base = 10);
		static String valueOf(unsigned char c, int base = 10);
		static String valueOf(short s, int base = 10);
		static String valueOf(unsigned short s, int base = 10);
		static String valueOf(int i, int base = 10);
		static String valueOf(unsigned int i, int base = 10);
		static String valueOf(long l, int base = 10);
		static String valueOf(unsigned long l, int base = 10);
		static String valueOf(long long l, int base = 10);
		static String valueOf(unsigned long long l, int base = 10);
		static String valueOf(float f, bool scientific = false);
		static String valueOf(double f, bool scientific = false);
		static String valueOf(long double d, bool scientific = false);

	private:
		size_t _replaceFirst(char oldChar, char newChar, size_t startIndex = 0);
		size_t _replaceFirst(const char* oldString, const char* newString, size_t startIndex = 0);
		size_t _replaceFirst(const String& oldString, const String& newString, size_t startIndex = 0);

		size_t _replaceFirstIgnoreCase(char oldChar, char newChar, size_t startIndex = 0);
		size_t _replaceFirstIgnoreCase(const char* oldString, const char* newString, size_t startIndex = 0);
		size_t _replaceFirstIgnoreCase(const String& oldString, const String& newString, size_t startIndex = 0);

		size_t mSize;
		char* mData;
	};

	bool operator<<(String& string, std::istream& file);
	bool operator<<(String& string, std::istream&& file);
	std::ostream& operator<<(std::ostream& stream, const String& string);
	String operator+(const char* string1, const String& string2);
	String operator+(const String& string1, const char* string2);
	String operator+(const String& string1, const String& string2);
	String operator+(const String& string1, char c);
	String operator+(char c, const String& string1);

} // namespace sonic
