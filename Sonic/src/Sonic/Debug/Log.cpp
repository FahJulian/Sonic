#include "Log.h"


namespace Sonic 
{
	Log Log::sInstance = Log();

	void Log::init(const String& filePath, std::ostream* ostream, const Level& consoleLevel, const Level& fileLevel)
	{
		sInstance.mFilePath = filePath;
		sInstance.mFileLevel = fileLevel.severity;
		sInstance.mConsoleLevel = consoleLevel.severity;
		sInstance.mFileStream = std::ofstream(filePath.getData());
		sInstance.mConsoleStream = ostream;
	}
}
