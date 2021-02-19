#include "UISlider.h"

using namespace Sonic;


Entity UI::createSlider(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const SliderProperties& properties, const EventListener<UISliderEvent>& onSlide)
{
	//	Entity background = scene->AddEntity();
	//	scene->AddComponent<UIComponent>(background, mode, x, y, width, height);

	//	if (properties.hasBackground)
	//		scene->AddComponent<UIRendererComponent>(background, properties.background);

	//	if (properties.hasCrossBar)
	//	{
	//		Entity crossbar = scene->AddEntity();
	//		scene->AddComponent<UIComponent>(crossbar, UISize::Mode::RelativeToEntity, 0.0f, (1.0f - properties.crossbarHeight) / 2.0f, 1.0f, properties.crossbarHeight, background);
	//		scene->AddComponent<UIRendererComponent>(crossbar, properties.crossbar);
	//	}

	//	Entity slider = scene->AddEntity();
	//	scene->AddComponent<UIComponent>(slider, UISize::Mode::RelativeToEntity, (1.0f - properties.sliderWidth) / 2.0f, 0.0f, properties.sliderWidth, 1.0f, background);
	//	scene->AddComponent<UIRendererComponent>(slider, properties.slider);
	//	scene->AddComponent<UIPositionConstraintsComponent>(slider, 0.0f, 0.0f, 1.0f - properties.sliderWidth, 0.0f);

	//	float sliderWidth = properties.sliderWidth;
	//	scene->AddComponent<UIMovableComponent>(slider, [scene, slider, background, onSlide, sliderWidth](const UIEntityMovedEvent& e) {
	//		auto* sliderComponent = scene->GetComponent<UIComponent>(slider);
	//		auto* backgroundComponent = scene->GetComponent<UIComponent>(background);

	//		float percentage = (sliderComponent->x - backgroundComponent->GetX()) / (backgroundComponent->GetWidth() * (1.0f - sliderWidth));
	//		onSlide(UISliderEvent{ percentage });
	//	}, StandardCursors::Link);

	//	scene->AddComponent<UIClickListenerComponent>(background, [scene, slider, background, onSlide, sliderWidth](const MouseButtonReleasedEvent& e) {
	//		auto* sliderComponent = scene->GetComponent<UIComponent>(slider);
	//		auto* backgroundComponent = scene->GetComponent<UIComponent>(background);

	//		scene->GetUIHandler().SetX(slider, sliderComponent, e.x - sliderComponent->GetWidth() / 2, backgroundComponent);

	//		float percentage = (sliderComponent->GetX() - backgroundComponent->GetX()) / (backgroundComponent->GetWidth() * (1.0f - sliderWidth));
	//		onSlide(UISliderEvent{ percentage });
	//	});

	//	return background;
	return 0;
}