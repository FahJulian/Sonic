#pragma once

#include <ostream>

#include "DynamicArray.h"

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

		String(size_t size);

		String(const String& other);

		String(String&& other) noexcept;

		~String();

		String& operator=(const String& other);

		String& operator=(String&& other) noexcept;

		operator const char* () const
		{
			return mData;
		}

		operator char* ()
		{
			return mData;
		}

		char& operator[](size_t index)
		{
			return get(index);
		}

		void operator+=(const String& string)
		{
			append(string);
		}
		void operator+=(const char* string)
		{
			append(string);
		}

		bool operator==(const String& other)
		{
			return equals(other);
		}
		bool operator==(const char* other)
		{
			return equals(other);
		}

		bool operator!=(const String& other)
		{
			return !equals(other);
		}
		bool operator!=(const char* other)
		{
			return !equals(other);
		}

		void append(const String& string);
		void append(const char* string);

		void insert(size_t index, const String& string);
		void insert(size_t index, const char* string);

		bool equals(const String& other) const;
		bool equals(const char* other) const;

		bool equalsIgnoreCase(const String& other) const;
		bool equalsIgnoreCase(const char* other) const;

		bool contains(char c);
		bool contains(const String& other) const;
		bool contains(const char* other) const;

		bool containsIgnoreCase(char c);
		bool containsIgnoreCase(const String& string) const;
		bool containsIgnoreCase(const char* string) const;

		bool endsWith(const String& string) const;
		bool endsWith(const char* string) const;

		bool startsWith(const String& string) const;
		bool startsWith(const char* string) const;

		bool replaceAll(char oldChar, char newChar);
		bool replaceAll(const char* oldString, const char* newString);
		bool replaceAll(const String& oldString, const String& newString);

		bool replaceFirst(char firstChar, char newChar);
		bool replaceFirst(const char* oldString, const char* newString);
		bool replaceFirst(const String& oldString, const String& newString);

		bool replaceLast(char firstChar, char newChar);
		bool replaceLast(const char* oldString, const char* newString);
		bool replaceLast(const String& oldString, const String& newString);

		size_t findFirst(char c) const;
		size_t findFirst(const char* string) const;
		size_t findFirst(const String& string) const;

		size_t findLast(char c) const;
		size_t findLast(const char* string) const;
		size_t findLast(const String& string) const;

		DynamicArray<String> split(char c) const;
		DynamicArray<String> split(const char* string) const;
		DynamicArray<String> split(const String& string) const;

		String subString(size_t beginIndex) const;
		String subString(size_t beginIndex, size_t endIndex) const;

		String trim() const;

		String toLowerCase() const;

		String toUpperCase() const;

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

		void setSize(size_t size);

		size_t getSize() const
		{
			return mSize;
		}

		char& get(size_t index);

		char* getData()
		{
			return mData;
		}

		const char* getData() const
		{
			return mData;
		}

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
		size_t mSize;
		char* mData;
	};

	std::ostream& operator<<(std::ostream& stream, const String& string);

} // namespace Sonic
