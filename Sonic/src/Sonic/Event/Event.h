#pragma once

/**
* Adds a static method that returs the unique ID of the event struct
*/
#define SONIC_REGISTER_EVENT \
	/* \
	* @return The unique ID of this event struct \
	*/ \
	static Sonic::EventType getEventType() \
	{ \
		static const Sonic::EventType type = Sonic::getNextEventType(); \
		return type; \
	} 

namespace Sonic {

	/**
	* Unique ID for each event struct
	*/
	using EventType = unsigned short;

	/**
	* @return The event type the next registered event struct gets assigned
	*/
	EventType getNextEventType();

}
