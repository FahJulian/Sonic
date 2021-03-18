#pragma once

#include <ostream>
#include <fstream>
#include <iostream>

#include "Sonic/Util/String.h"

namespace sonic 
{
	class Log
	{
	private:
		struct Level
		{
			operator uint8_t() const { return severity; }

			const uint8_t severity;
			const char* const color;
			const char* const name;
		};

		static constexpr const char* const ANSI_RESET = "\033[0m";

	public:
		static constexpr const Level& FATAL = { 5, "\x1B[1m\x1B[41m", "[FATAL] " }; 
		static constexpr const Level& ERROR = { 4, "\x1B[1m\x1B[31m", "[ERROR] " };
		static constexpr const Level& WARN = { 3, "\x1B[1m\x1B[33m", "[WARN]  " };
		static constexpr const Level& INFO = { 2, "\x1B[1m\x1B[36m", "[INFO]  " };
		static constexpr const Level& DEBUG = { 1, "\x1B[1m\x1B[35m", "[DEBUG] " };
		static constexpr const Level& TRACE = { 0, "\x1B[1m\x1B[37m", "[TRACE] " };

		static constexpr uint8_t DISABLED = 0xff;
		static constexpr uint8_t ALL = 0;

		static void init(const String& filePath, std::ostream* ostream, uint8_t consoleLevel, uint8_t fileLevel);

		template<typename Arg, typename... Args>
		static void log(const Level& level, const Arg& arg, const Args&... args)
		{
			if (level.severity >= sInstance.mConsoleLevel)
			{
				*sInstance.mConsoleStream << level.color;
				pushMessage(*sInstance.mConsoleStream, level, arg, (args)...);
				*sInstance.mConsoleStream << ANSI_RESET;
			}

			if (level.severity >= sInstance.mFileLevel)
				pushMessage(sInstance.mFileStream, level, arg, (args)...);
		}

		static void writeToFile();

	private:
		Log()
			: mFilePath(), mConsoleLevel(Log::DISABLED), mFileLevel(Log::DISABLED), mConsoleStream(nullptr), mFileStream()
		{
		}

		template<typename Arg, typename... Args>
		inline static void pushMessage(std::ostream& stream, const Level& level, const Arg& arg, const Args&... args)
		{
			stream << level.name;
			stream << arg;
			((stream << args), ...);
			stream << std::endl;
		}

		String mFilePath;
		uint8_t mConsoleLevel;
		uint8_t mFileLevel;
		std::ostream* mConsoleStream;
		std::ofstream mFileStream;

		static Log sInstance;
	};

} // namespace sonic

