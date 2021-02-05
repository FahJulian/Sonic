#include <cmath>
#include "Sonic/Window/Window.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/Entity.h"
#include "UI.h"
#include "UIComponents.h"

using namespace Sonic;


namespace Sonic::UI {

	Entity createButton(Scene* scene, float x, float y, float zIndex, float width, float height, const Color& color, 
		const Color& hoverColor, float borderWeight, const Color& borderColor, float edgeRadius, EventListener<MouseButtonReleasedEvent> onClick)
	{
		Entity button = scene->AddEntity();
		button.AddComponent<UIComponent>(UISize::Mode::Absolute, x, y, width, height, zIndex);
		button.AddComponent<UIRendererComponent>(UIRendererProperties(color).Border(borderColor, borderWeight).EdgeRadius(edgeRadius));
		button.AddComponent<UIHoverComponent>(UIRendererProperties(hoverColor).Border(borderColor, borderWeight).EdgeRadius(edgeRadius));
		button.AddComponent<UIClickListenerComponent>(onClick);
		return button;
	}

	Entity createButton(Scene* scene, float x, float y, float width, float height,
		const Color& color, const Color& hoverColor, EventListener<MouseButtonReleasedEvent> onClick)
	{
		Entity button = scene->AddEntity();
		button.AddComponent<UIComponent>(UISize::Mode::Absolute, x, y, width, height);
		button.AddComponent<UIRendererComponent>(color);
		button.AddComponent<UIHoverComponent>(hoverColor);
		button.AddComponent<UIClickListenerComponent>(onClick);
		return button;
	}

}
