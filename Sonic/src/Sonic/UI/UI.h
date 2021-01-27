#pragma once
#include "Sonic/Scene/EntityID.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/Components.h"
#include "UISize.h"

namespace Sonic {

	class Scene;
	class Entity;

	namespace UI {

		Entity createButton(Scene* scene, float x, float y, float zIndex, float width, float height, const Color& color, 
			const Color& hoverColor, float borderWeight, const Color& borderColor, float edgeRadius, EventListener<MouseButtonReleasedEvent> onClick);

		Entity createButton(Scene* scene, float x, float y, float width, float height,
			const Color& color, const Color& hoverColor, EventListener<MouseButtonReleasedEvent> onClick);

		float toAbsoluteX(Scene* scene, EntityID parent, const UISize& x);
		float toAbsoluteY(Scene* scene, EntityID parent, const UISize& y);
		float toAbsoluteWidth(Scene* scene, EntityID parent, const UISize& width);
		float toAbsoluteHeight(Scene* scene, EntityID parent, const UISize& height);
	}

}
