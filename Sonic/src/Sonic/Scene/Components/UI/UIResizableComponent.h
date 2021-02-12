#pragma once
#include <stdint.h>
#include "Sonic/Scene/UI/UISize.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events/UIEvents.h"

namespace Sonic {

	struct UIResizableComponent
	{
		struct Borders
		{
			bool bottom;
			bool top;
			bool left;
			bool right;

			operator bool()
			{
				return *reinterpret_cast<uint32_t*>(this) != 0;
			}

			bool operator=(bool b)
			{
				bottom = b;
				top = b;
				left = b;
				right = b;
				return b;
			}
		};

		float grabSize;

		Borders bordersResizable;
		Borders bordersHovered = { false, false, false, false };

		EventListener<UIEntityResizedEvent> onResized;

	public:
		UIResizableComponent(float grabSize = 3.0f, Borders bordersResizable = { true, true, true, true })
			: grabSize(grabSize), bordersResizable(bordersResizable), onResized(nullptr)
		{
		}

		UIResizableComponent(EventListener<UIEntityResizedEvent> onResized, float grabSize = 3.0f, Borders bordersResizable = { true, true, true, true })
			: grabSize(grabSize), bordersResizable(bordersResizable), onResized(onResized)
		{
		}

		friend class SceneUIHandler;
	};

}
