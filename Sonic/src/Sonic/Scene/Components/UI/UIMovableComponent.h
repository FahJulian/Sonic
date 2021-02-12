#pragma once
#include "Sonic/Window/Cursor/Cursors.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events/UIEvents.h"

namespace Sonic {

	struct UIMovableComponent
	{
	private:
		StandardCursor cursor;
		Ref<EventListener<UIEntityMovedEvent>> onMoved;
		bool hovered = false;

	public:
		UIMovableComponent(StandardCursor cursor = StandardCursors::Move)
			: cursor(cursor), onMoved(createRef<EventListener<UIEntityMovedEvent>>(nullptr))
		{
		}

		UIMovableComponent(EventListener<UIEntityMovedEvent> onMoved, StandardCursor cursor = StandardCursors::Move)
			: cursor(cursor), onMoved(createRef<EventListener<UIEntityMovedEvent>>(onMoved))
		{
		}

		friend class SceneUIHandler;
	};

}
