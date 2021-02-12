#pragma once
#include "Sonic/Scene/ECS/Entity.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Renderer/UI/UIRendererProperties.h"
#include "Sonic/Event/Events/UIEvents.h"
#include "UISize.h"

namespace Sonic {

	class Scene;

	namespace UI {

		struct SliderProperties
		{
			bool hasBackground = false;
			UIRendererProperties background;

			bool hasCrossBar = false;
			UIRendererProperties crossbar;
			float crossbarHeight = 0.05f;

			UIRendererProperties slider;
			float sliderWidth = 0.05f;

			SliderProperties(UIRendererProperties slider, float sliderWidth = 0.05f)
				: slider(slider), sliderWidth(sliderWidth)
			{
			}

			SliderProperties(UIRendererProperties slider, UIRendererProperties background, float sliderWidth = 0.05f)
				: background(background), slider(slider), sliderWidth(sliderWidth)
			{
			}

			SliderProperties(UIRendererProperties slider, UIRendererProperties background, UIRendererProperties crossbar, float sliderWidth = 0.05f, float crossbarHeight = 0.05f)
				: background(background), slider(slider), crossbar(crossbar), crossbarHeight(crossbarHeight), sliderWidth(sliderWidth)
			{
			}
		};

		Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties, 
			EventListener<MouseButtonReleasedEvent> onClick);
		Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties, 
			const UIRendererProperties& hoveredProperties, EventListener<MouseButtonReleasedEvent> onClick);
		Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
			EventListener<MouseButtonReleasedEvent> onClick, const Font& font, const Color& textColor, const std::string& text);
		Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
			const UIRendererProperties& hoveredProperties, EventListener<MouseButtonReleasedEvent> onClick, const Font& font, const Color& textColor, const std::string& text);

		Entity createSlider(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const SliderProperties& properties, const EventListener<UISliderEvent>& onSlide);
	}

}
