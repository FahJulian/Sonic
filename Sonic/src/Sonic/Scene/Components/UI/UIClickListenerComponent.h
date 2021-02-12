#pragma once
#include "Sonic/Base.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events.h"

namespace Sonic {

	struct UIClickListenerComponent
	{
		bool clicked = false;
		Ref<EventListener<MouseButtonReleasedEvent>> listener;

		UIClickListenerComponent(EventListener<MouseButtonReleasedEvent> listener)
			: listener(createRef<EventListener<MouseButtonReleasedEvent>>(listener))
		{
		}
	};

}
