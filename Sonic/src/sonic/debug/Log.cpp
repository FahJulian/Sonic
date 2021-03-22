#include "Log.h"

#include "sonic/Base.h"

namespace sonic 
{
	Log* Log::sInstance;

	void Log::init(String filePath, std::ostream* ostream, uint8_t consoleLevel, uint8_t fileLevel)
	{
		static Log instance;
		sInstance = &instance;

		sInstance->mFilePath = filePath;
		sInstance->mFileLevel = fileLevel;
		sInstance->mConsoleLevel = consoleLevel;
		sInstance->mFileStream = std::ofstream(filePath, std::ios::out);
		sInstance->mConsoleStream = ostream;
	}

	void Log::init(Log* instance)
	{
		SN_ASSERT(instance != nullptr, "Cant initialize Log without another valid Log instance");

		sInstance = instance;
	}
} // namsInstance->mConsoleStreamespace sonic
