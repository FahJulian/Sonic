#include "UI.h"
#include "UIComponents.h"

namespace Sonic {

	namespace UI {

		Entity createButton(Scene* scene, float x, float y, float zIndex, float width, float height, const Color& color, 
			const Color& hoverColor, float borderWeight, const Color& borderColor, float edgeRadius, EventListener<MouseButtonReleasedEvent> onClick)
		{
			Entity button = scene->AddEntity();
			button.AddComponent<UIConstraintsComponent>(x, y, zIndex, width, height);
			button.AddComponent<UIRendererComponent>(color);
			button.AddComponent<UIBorderComponent>(borderWeight, borderColor);
			button.AddComponent<UIRoundedEdgeComponent>(edgeRadius);
			button.AddComponent<UIHoverComponent>(hoverColor);
			button.AddComponent<UIClickListenerComponent>(onClick);
			return button;
		}

		Entity createButton(Scene* scene, float x, float y, float width, float height,
			const Color& color, const Color& hoverColor, EventListener<MouseButtonReleasedEvent> onClick)
		{
			Entity button = scene->AddEntity();
			button.AddComponent<UIConstraintsComponent>(x, y, width, height);
			button.AddComponent<UIRendererComponent>(color);
			button.AddComponent<UIHoverComponent>(hoverColor);
			button.AddComponent<UIClickListenerComponent>(onClick);
			return button;
		}

	}

}
