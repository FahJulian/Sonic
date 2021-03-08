#pragma once
#include "Sonic/Window/Window.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events/UIEvents.h"

namespace Sonic {

	struct UIMovableComponent
	{
	private:
		String cursor;
		Ref<EventListener<UIEntityMovedEvent>> onMoved;
		bool hovered = false;

	public:
		UIMovableComponent(String cursor = Cursors::Move)
			: cursor(cursor), onMoved(createRef<EventListener<UIEntityMovedEvent>>(nullptr))
		{
		}

		UIMovableComponent(EventListener<UIEntityMovedEvent> onMoved, String cursor = Cursors::Move)
			: cursor(cursor), onMoved(createRef<EventListener<UIEntityMovedEvent>>(onMoved))
		{
		}

		friend class SceneUIHandler;
	};

}
