#pragma once
#include "Sonic/Base.h"
#include <string>

namespace Sonic {
	
	namespace Util {

		unsigned char* loadCursor(String filePath, int& width, int& height, int& offsetX, int& offsetY, bool flipVertically = true);

	}

}
