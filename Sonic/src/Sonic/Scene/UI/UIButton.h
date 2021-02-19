#pragma once
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/ECS/Entity.h"
#include "Sonic/Event/EventListener.h"
#include "Sonic/Event/Events/UIEvents.h"
#include "UISize.h"

namespace Sonic {

	namespace UI {

		struct ButtonProperties
		{
			UISize x, y, width, height;
		};

		Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
			EventListener<MouseButtonReleasedEvent> onClick);
		Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
			const UIRendererProperties& hoveredProperties, EventListener<MouseButtonReleasedEvent> onClick);
		Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
			EventListener<MouseButtonReleasedEvent> onClick, const Font& font, const Color& textColor, const String& text);
		Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
			const UIRendererProperties& hoveredProperties, EventListener<MouseButtonReleasedEvent> onClick, const Font& font, const Color& textColor, const String& text);

	}

}