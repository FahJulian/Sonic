#include "Sonic/Scene/Components/UIComponents.h"
#include "UIButton.h"

using namespace Sonic;


Entity UI::createButton(Scene* scene, UISize x, UISize y, UISize width, UISize height, const UIRendererProperties& properties,
	const EventListener<MouseButtonReleasedEvent>& listener, Entity parent)
{
	Entity button = scene->AddEntity();
	scene->AddComponent<UIComponent>(button, x, y, width, height, parent);
	scene->AddComponent<UIRendererComponent>(button, properties);
	//scene->AddComponent<UIClickListenerComponent>(button, listener);

	if (parent != 0)
		scene->AddToGroup(parent, button);

	return button;
}

Entity UI::createButton(Scene* scene, UISize x, UISize y, UISize width, UISize height, const UIRendererProperties& properties,
	const UIRendererProperties& hoveredProperties, const EventListener<MouseButtonReleasedEvent>& listener, Entity parent)
{
	Entity button = createButton(scene, x, y, width, height, properties, listener, parent);
	scene->AddComponent<UIHoverComponent>(button, hoveredProperties);
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
	const UIRendererProperties& hoveredProperties, const EventListener<MouseButtonReleasedEvent>& listener,
	const Font& font, const Color& textColor, const String& text, Entity parent)
{
	Entity button = createButton(scene, x, y, width, height, properties, hoveredProperties, listener, parent);
	scene->AddComponent<UITextComponent>(button, font, textColor, text);
	return button;
}

Entity UI::createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
	const EventListener<MouseButtonReleasedEvent>& listener, Entity parent)
{
	return createButton(scene, UISize{ mode, x }, UISize{ mode, y }, UISize{ mode, width }, UISize{ mode, height }, properties, listener, parent);
}

Entity UI::createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
	const UIRendererProperties& hoveredProperties, const EventListener<MouseButtonReleasedEvent>& listener, Entity parent)
{
	return createButton(scene, UISize{ mode, x }, UISize{ mode, y }, UISize{ mode, width }, UISize{ mode, height }, properties, hoveredProperties, listener, parent);
}

Entity UI::createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
	const EventListener<MouseButtonReleasedEvent>& listener,
	const Font& font, const Color& textColor, const String& text, Entity parent)
{
	return createButton(scene, UISize{ mode, x }, UISize{ mode, y }, UISize{ mode, width }, UISize{ mode, height }, properties, listener, font, textColor, text, parent);
}

Entity UI::createButton(Scene* scene, UISize::Mode mode, float x, float y, float width, float height, const UIRendererProperties& properties,
	const UIRendererProperties& hoveredProperties, const EventListener<MouseButtonReleasedEvent>& listener,
	const Font& font, const Color& textColor, const String& text, Entity parent)
{
	return createButton(scene, UISize{ mode, x }, UISize{ mode, y }, UISize{ mode, width }, UISize{ mode, height }, properties, hoveredProperties, listener, font, textColor, text, parent);
}
