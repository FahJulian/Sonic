#pragma once
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/ECS/Entity.h"
#include "Sonic/Event/EventListener.h"
#include "Sonic/Event/Events/UIEvents.h"
#include "UISize.h"

namespace Sonic {

	namespace UI {

		Entity createButton(Scene* scene, UISize x, UISize y, UISize width, UISize height, const UIRendererProperties& properties, 
			const EventListener<MouseButtonReleasedEvent>& listener, Entity parent = 0);
		Entity createButton(Scene* scene, UISize x, UISize y, UISize width, UISize height, const UIRendererProperties& properties, 
			const UIRendererProperties& hovereredProperties, const EventListener<MouseButtonReleasedEvent>& listener, Entity parent = 0);
		Entity createButton(Scene* scene, UISize x, UISize y, UISize width, UISize height, const UIRendererProperties& properties,
			const EventListener<MouseButtonReleasedEvent>& listener, 
			const Font& font, const Color& textColor, const String& text, Entity parent = 0);
		Entity createButton(Scene* scene, UISize x, UISize y, UISize width, UISize height, const UIRendererProperties& properties,
			const UIRendererProperties& hovereredProperties, const EventListener<MouseButtonReleasedEvent>& listener, 
			const Font& font, const Color& textColor, const String& text, Entity parent = 0);

	}

}