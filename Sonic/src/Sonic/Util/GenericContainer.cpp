#include "GenericContainer.h"

using namespace Sonic;

namespace Sonic::GenericContainer {

	Key nextKey()
	{
		static Key key = 0;
		return key++;
	}

}
