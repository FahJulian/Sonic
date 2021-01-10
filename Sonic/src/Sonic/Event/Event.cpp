#pragma once
#include "Event.h"

static Sonic::EventType nextEventType = 1;

namespace Sonic {

	EventType getNextEventType()
	{
		return nextEventType++;
	}

}
