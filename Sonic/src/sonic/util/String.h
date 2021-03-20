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

		operator const char* () const { return mData; }

		operator char* () { return mData; }

		char& operator[](size_t index) { return get(index); }
		const char& operator[](size_t index) const { return get(index); }

		void operator+=(const String& string) { append(string); }
		void operator+=(const char* string) { append(string); }

		bool operator==(const String& other) const { return equals(other); }
		bool operator==(const char* other) const { return equals(other); }

		bool operator!=(const String& other) const { return !equals(other); }
		bool operator!=(const char* other) const { return !equals(other); }

		void append(const String& string);
		void append(const char* string);

		void insert(size_t index, const String& string);
		void insert(size_t index, const char* string);

		bool equals(const String& other) const;
		bool equals(const char* other) const;

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

		bool replaceAll(char oldChar, char newChar);
		bool replaceAll(const char* oldString, const char* newString);
		bool replaceAll(const String& oldString, const String& newString);

		bool replaceAllIgnoreCase(char oldChar, char newChar);
		bool replaceAllIgnoreCase(const char* oldString, const char* newString);
		bool replaceAllIgnoreCase(const String& oldString, const String& newString);

		size_t replaceFirst(char oldChar, char newChar, size_t startIndex = 0);
		size_t replaceFirst(const char* oldString, const char* newString, size_t startIndex = 0);
		size_t replaceFirst(const String& oldString, const String& newString, size_t startIndex = 0);

		size_t replaceFirstIgnoreCase(char oldChar, char newChar, size_t startIndex = 0);
		size_t replaceFirstIgnoreCase(const char* oldString, const char* newString, size_t startIndex = 0);
		size_t replaceFirstIgnoreCase(const String& oldString, const String& newString, size_t startIndex = 0);

		size_t replaceLast(char oldChar, char newChar);
		size_t replaceLast(const char* oldString, const char* newString);
		size_t replaceLast(const String& oldString, const String& newString);

		size_t replaceLastIgnoreCase(char oldChar, char newChar);
		size_t replaceLastIgnoreCase(const char* oldString, const char* newString);
		size_t replaceLastIgnoreCase(const String& oldString, const String& newString);

		size_t findFirst(char c, size_t startIndex = 0) const;
		size_t findFirst(const char* string, size_t startIndex = 0) const;
		size_t findFirst(const String& string, size_t startIndex = 0) const;

		size_t findFirstIgnoreCase(char c, size_t startIndex = 0) const;
		size_t findFirstIgnoreCase(const char* string, size_t startIndex = 0) const;
		size_t findFirstIgnoreCase(const String& string, size_t startIndex = 0) const;

		size_t findLast(char c) const;
		size_t findLast(const char* string) const;
		size_t findLast(const String& string) const;

		size_t findLastIgnoreCase(char c) const;
		size_t findLastIgnoreCase(const char* string) const;
		size_t findLastIgnoreCase(const String& string) const;

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
		size_t mSize;
		char* mData;
	};

	std::ostream& operator<<(std::ostream& stream, const String& string);

} // namespace sonic
