#pragma once

#define SONIC_REGISTER_EVENT \
	static Sonic::EventType getEventType() \
	{ \
		static const Sonic::EventType type = Sonic::getNextEventType(); \
		return type; \
	} 

namespace Sonic {

	using EventType = unsigned short;

	EventType getNextEventType();

}
