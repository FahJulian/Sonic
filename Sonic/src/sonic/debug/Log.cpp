#include "Log.h"

#include <fstream>

#include "sonic/Base.h"

#include "sonic/util/platform/FileUtils.h"

namespace sonic
{
	Log* Log::sInstance;

	Log::~Log()
	{
		platform::createDirectoryRecursively(mFilePath.subString(0, mFilePath.replaceAll('/', '\\').findLastOf('\\') + 1));

		std::ofstream file = std::ofstream(mFilePath.getData(), std::ios::out);
		file << mFileStream.str();
		file.close();
	}

	void Log::init(String filePath, std::ostream& ostream, uint8_t consoleLevel, uint8_t fileLevel)
	{
		static Log instance;
		sInstance = &instance;

		sInstance->mFilePath = filePath;
		sInstance->mFileLevel = fileLevel;
		sInstance->mConsoleLevel = consoleLevel;
		sInstance->mFileStream = std::ostringstream();
		sInstance->mConsoleStream = &ostream;
	}

	void Log::init(Log* instance)
	{
		SN_ASSERT(instance != nullptr, "Cant initialize Log without another valid Log instance");

		sInstance = instance;
	}

} // namespace sonic
