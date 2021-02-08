#include <cmath>
#include "Sonic/Window/Window.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/Entity.h"
#include "UI.h"
#include "UIComponents.h"

using namespace Sonic;


namespace Sonic::UI {

	Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
		EventListener<MouseButtonReleasedEvent> onClick)
	{
		Entity button = scene->AddEntity();
		button.AddComponent<UIComponent>(mode, x, y, width, height);
		button.AddComponent<UIRendererComponent>(properties);
		button.AddComponent<UIClickListenerComponent>(onClick);
		return button;
	}

	Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
		const UIRendererProperties& hoveredProperties, EventListener<MouseButtonReleasedEvent> onClick)
	{
		Entity button = createButton(scene, mode, x, y, width, height, properties, onClick);
		button.AddComponent<UIHoverComponent>(hoveredProperties);
		return button;
	}

	Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
		EventListener<MouseButtonReleasedEvent> onClick, const Font& font, const Color& textColor, const std::string& text)
	{
		Entity button = createButton(scene, mode, x, y, width, height, properties, onClick);
		button.AddComponent<UITextComponent>(font, textColor, text);
		return button;
	}

	Entity createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
		const UIRendererProperties& hoveredProperties, EventListener<MouseButtonReleasedEvent> onClick, const Font& font, const Color& textColor, const std::string& text)
	{
		Entity button = createButton(scene, mode, x, y, width, height, properties, hoveredProperties, onClick);
		button.AddComponent<UITextComponent>(font, textColor, text);
		return button;
	}

	Entity createSlider(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const SliderProperties& properties, const EventListener<UISliderEvent>& onSlide)
	{
		Entity background = scene->AddEntity();
		background.AddComponent<UIComponent>(mode, x, y, width, height);

		if (properties.hasBackground)
			background.AddComponent<UIRendererComponent>(properties.background);

		if (properties.hasCrossBar)
		{
			Entity crossbar = scene->AddEntity();
			crossbar.AddComponent<UIComponent>(UISize::Mode::RelativeToEntity, 0.0f, (1.0f - properties.crossbarHeight) / 2.0f, 1.0f, properties.crossbarHeight, background);
			crossbar.AddComponent<UIRendererComponent>(properties.crossbar);
		}

		Entity slider = scene->AddEntity();
		slider.AddComponent<UIComponent>(UISize::Mode::RelativeToEntity, (1.0f - properties.sliderWidth) / 2.0f, 0.0f, properties.sliderWidth, 1.0f, background);
		slider.AddComponent<UIRendererComponent>(properties.slider);
		slider.AddComponent<UIPositionConstraintsComponent>(0.0f, 0.0f, 1.0f - properties.sliderWidth, 0.0f);

		float sliderWidth = properties.sliderWidth;
		slider.AddComponent<UIMovableComponent>([scene, slider, background, onSlide, sliderWidth](const UIEntityMovedEvent& e) {
			auto* sliderComponent = scene->GetComponent<UIComponent>(slider);
			auto* backgroundComponent = scene->GetComponent<UIComponent>(background);

			float percentage = (sliderComponent->GetX() - backgroundComponent->GetX()) / (backgroundComponent->GetWidth() * (1.0f - sliderWidth));
			onSlide(UISliderEvent{ percentage });
		}, StandardCursors::Link);

		background.AddComponent<UIClickListenerComponent>([scene, slider, background, onSlide, sliderWidth](const MouseButtonReleasedEvent& e) {
			auto* sliderComponent = scene->GetComponent<UIComponent>(slider);
			auto* backgroundComponent = scene->GetComponent<UIComponent>(background);

			scene->GetUIHandler().SetX(slider, sliderComponent, e.x - sliderComponent->GetWidth() / 2, backgroundComponent);

			float percentage = (sliderComponent->GetX() - backgroundComponent->GetX()) / (backgroundComponent->GetWidth() * (1.0f - sliderWidth));
			onSlide(UISliderEvent{ percentage });
		});

		return background;
	}

}
