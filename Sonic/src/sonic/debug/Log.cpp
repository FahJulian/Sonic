#include "Log.h"

namespace sonic 
{
	Log Log::sInstance = Log();

	void Log::init(String filePath, std::ostream* ostream, uint8_t consoleLevel, uint8_t fileLevel)
	{
		sInstance.mFilePath = filePath;
		sInstance.mFileLevel = fileLevel;
		sInstance.mConsoleLevel = consoleLevel;
		sInstance.mFileStream = std::ofstream(filePath, std::ios::out);
		sInstance.mConsoleStream = ostream;
	}

} // namespace sonic
