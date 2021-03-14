#pragma once
#include "Sonic/Base.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Util/Callable.h"

namespace Sonic {

	struct UIClickListenerComponent
	{
		bool clicked = false;
		Ref<Callable<void(const UIEntityClickedEvent&)>> listener;

		UIClickListenerComponent(Ref<Callable<void(const UIEntityClickedEvent&)>>)
			: listener(listener)
		{
		}
	};

}
