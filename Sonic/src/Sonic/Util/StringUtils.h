#pragma once
#include "Sonic/Base.h"

namespace Sonic::Util
{

	inline String toNarrowString(const WideString& wideString)
	{
#pragma warning(disable:4244)
		return String(wideString.begin(), wideString.end());
#pragma warning(default:4244)
	}

	inline WideString toWideString(const String& string)
	{
		return WideString(string.begin(), string.end());
	}

	template<typename T>
	inline String toString(T t)
	{
		return std::to_string(t);
	}

	inline bool endsWith(const String& string, const String& ending)
	{
		return string.substr(string.size() - ending.size()) == ending;
	}

	inline bool beginsWith(const String& string, const String& beginning)
	{
		return string.substr(0, beginning.size()) == beginning;
	}

	inline String getFileNamePrefix(const String& filePath)
	{
		String it = filePath.substr(filePath.find_last_of('/') + 1);
		return it.substr(0, it.find_first_of('.'));
	}
}
