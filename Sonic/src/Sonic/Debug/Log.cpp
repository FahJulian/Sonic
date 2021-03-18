#include "Log.h"


namespace sonic 
{
	Log Log::sInstance = Log();

	void Log::init(const String& filePath, std::ostream* ostream, uint8_t consoleLevel, uint8_t fileLevel)
	{
		sInstance.mFilePath = filePath;
		sInstance.mFileLevel = fileLevel;
		sInstance.mConsoleLevel = consoleLevel;
		sInstance.mFileStream = std::ofstream(filePath.getData());
		sInstance.mConsoleStream = ostream;
	}

	void Log::writeToFile()
	{
		sInstance.mFileStream.close();
		if (!sInstance.mFileStream.good())
			*sInstance.mConsoleStream << WARN.color << "Could not write log to file " << sInstance.mFilePath << ANSI_RESET << std::endl;

		sInstance.mFileStream = std::ofstream(sInstance.mFilePath.getData());
	}

} // namespace sonic
