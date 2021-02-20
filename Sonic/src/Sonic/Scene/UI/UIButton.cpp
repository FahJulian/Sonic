#include "Sonic/Scene/Components/UIComponents.h"
#include "UIButton.h"

using namespace Sonic;


Entity UI::createButton(Scene* scene, UISize x, UISize y, UISize width, UISize height, const UIRendererProperties& properties,
	const EventListener<MouseButtonReleasedEvent>& listener, Entity parent)
{
	Entity button = scene->AddEntity();
	scene->AddComponent<UIComponent>(button, x, y, width, height, parent);
	scene->AddComponent<UIRendererComponent>(button, properties);
	scene->AddComponent<UIClickListenerComponent>(button, listener);
	return button;
}

Entity UI::createButton(Scene* scene, UISize x, UISize y, UISize width, UISize height, const UIRendererProperties& properties,
	const UIRendererProperties& hovereredProperties, const EventListener<MouseButtonReleasedEvent>& listener, Entity parent)
{
	Entity button = createButton(scene, x, y, width, height, properties, listener, parent);
	scene->AddComponent<UIHoverComponent>(button, hovereredProperties);
	return button;
}

Entity UI::createButton(Scene* scene, UISize x, UISize y, UISize width, UISize height, const UIRendererProperties& properties,
	const EventListener<MouseButtonReleasedEvent>& listener,
	const Font& font, const Color& textColor, const String& text, Entity parent)
{
	Entity button = createButton(scene, x, y, width, height, properties, listener, parent);
	scene->AddComponent<UITextComponent>(button, font, textColor, text);
	return button;
}

Entity UI::createButton(Scene* scene, UISize x, UISize y, UISize width, UISize height, const UIRendererProperties& properties,
	const UIRendererProperties& hovereredProperties, const EventListener<MouseButtonReleasedEvent>& listener,
	const Font& font, const Color& textColor, const String& text, Entity parent)
{
	Entity button = createButton(scene, x, y, width, height, properties, hovereredProperties, listener, parent);
	scene->AddComponent<UITextComponent>(button, font, textColor, text);
	return button;
}
