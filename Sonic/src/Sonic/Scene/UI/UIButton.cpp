#include "Sonic/Scene/Components/UIComponents.h"
#include "UIButton.h"

using namespace Sonic;


Entity UI::createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
	EventListener<MouseButtonReleasedEvent> onClick)
{
	Entity button = scene->AddEntity();
	scene->AddComponent<UIComponent>(button, mode, x, y, width, height);
	scene->AddComponent<UIRendererComponent>(button, properties);
	scene->AddComponent<UIClickListenerComponent>(button, onClick);
	return button;
}

Entity UI::createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
	const UIRendererProperties& hoveredProperties, EventListener<MouseButtonReleasedEvent> onClick)
{
	Entity button = createButton(scene, mode, x, y, width, height, properties, onClick);
	scene->AddComponent<UIHoverComponent>(button, hoveredProperties);
	return button;
}

Entity UI::createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
	EventListener<MouseButtonReleasedEvent> onClick, const Font& font, const Color& textColor, const String& text)
{
	Entity button = createButton(scene, mode, x, y, width, height, properties, onClick);
	scene->AddComponent<UITextComponent>(button, font, textColor, text);
	return button;
}

Entity UI::createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
	const UIRendererProperties& hoveredProperties, EventListener<MouseButtonReleasedEvent> onClick, const Font& font, const Color& textColor, const String& text)
{
	Entity button = createButton(scene, mode, x, y, width, height, properties, hoveredProperties, onClick);
	scene->AddComponent<UITextComponent>(button, font, textColor, text);
	return button;
}
