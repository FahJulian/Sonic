#include <iostream>
#include "Log.h"
#include "Sonic/App.h"

namespace Sonic {

	void Log::debugAssert(bool b, const char* msg)
	{
		if (!b)
		{
			Log::debug(msg);
			App::get()->Stop();
		}
	}

	void Log::debug(const char* msg)
	{
		std::cout << "[DEBUG] " << msg << std::endl;
	}

}
