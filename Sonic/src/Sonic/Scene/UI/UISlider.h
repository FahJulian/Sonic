#pragma once
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/ECS/Entity.h"
#include "Sonic/Event/EventListener.h"
#include "Sonic/Event/Events/UIEvents.h"
#include "Sonic/Renderer/UI/UIRendererProperties.h"
#include "UISize.h"

namespace Sonic {

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

		Entity createSlider(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const SliderProperties& properties, const EventListener<UISliderEvent>& onSlide);

	}
}