#pragma once
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/Entity.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/Components.h"

namespace Sonic {

	namespace UI {

		Entity createButton(Scene* scene, float x, float y, float zIndex, float width, float height, const Color& color, 
			const Color& hoverColor, float borderWeight, const Color& borderColor, float edgeRadius, EventListener<MouseButtonReleasedEvent> onClick);

		Entity createButton(Scene* scene, float x, float y, float width, float height,
			const Color& color, const Color& hoverColor, EventListener<MouseButtonReleasedEvent> onClick);
	}

}
