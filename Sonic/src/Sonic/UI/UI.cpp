#include <cmath>
#include "Sonic/Window/Window.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/Entity.h"
#include "UI.h"
#include "UIComponents.h"

namespace Sonic {

	namespace UI {

		Entity createButton(Scene* scene, float x, float y, float zIndex, float width, float height, const Color& color, 
			const Color& hoverColor, float borderWeight, const Color& borderColor, float edgeRadius, EventListener<MouseButtonReleasedEvent> onClick)
		{
			Entity button = scene->AddEntity();
			button.AddComponent<UIComponent>(UISize::Mode::Absolute, x, y, zIndex, width, height);
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
			button.AddComponent<UIComponent>(UISize::Mode::Absolute, x, y, width, height);
			button.AddComponent<UIRendererComponent>(color);
			button.AddComponent<UIHoverComponent>(hoverColor);
			button.AddComponent<UIClickListenerComponent>(onClick);
			return button;
		}

		float toAbsoluteX(Scene* scene, EntityID parent, const UISize& x)
		{
			if (x.mode == UISize::Mode::Absolute)
				return x.value;
			else if (x.mode == UISize::Mode::RelativeToWindow)
				return x.value * Window::getWidth();
			else if (x.mode == UISize::Mode::RelativeToEntity)
			{
				SONIC_LOG_DEBUG_ASSERT(parent != 0, "UIComponent: Must spectify a parent entity if RelativeToEntity is the size mode");
				UIComponent* parentComponent = scene->GetComponent<UIComponent>(parent);
				return toAbsoluteX(scene, parentComponent->parent, parentComponent->x) + x.value * toAbsoluteWidth(scene, parentComponent->parent, parentComponent->width);
			}
			else
			{
				SONIC_LOG_DEBUG("Unknown UISize mode");
				return 0.0f;
			}
		}

		float toAbsoluteY(Scene* scene, EntityID parent, const UISize& y)
		{
			if (y.mode == UISize::Mode::Absolute)
				return y.value;
			else if (y.mode == UISize::Mode::RelativeToWindow)
				return y.value * Window::getHeight();
			else if (y.mode == UISize::Mode::RelativeToEntity)
			{
				SONIC_LOG_DEBUG_ASSERT(parent != 0, "UIComponent: Must spectify a parent entity if RelativeToEntity is the size mode");
				UIComponent* parentComponent = scene->GetComponent<UIComponent>(parent);
				return toAbsoluteY(scene, parentComponent->parent, parentComponent->y) + y.value * toAbsoluteHeight(scene, parentComponent->parent, parentComponent->height);
			}
			else
			{
				SONIC_LOG_DEBUG("Unknown UISize mode");
				return 0.0f;
			}
		}

		float toAbsoluteWidth(Scene* scene, EntityID parent, const UISize& width)
		{
			if (width.mode == UISize::Mode::Absolute)
				return width.value;
			else if (width.mode == UISize::Mode::RelativeToWindow)
				return width.value * Window::getWidth();
			else if (width.mode == UISize::Mode::RelativeToEntity)
			{
				SONIC_LOG_DEBUG_ASSERT(parent != 0, "UIComponent: Must spectify a parent entity if RelativeToEntity is the size mode");
				UIComponent* parentComponent = scene->GetComponent<UIComponent>(parent);
				return width.value * toAbsoluteWidth(scene, parentComponent->parent, parentComponent->width);
			}
			else
			{
				SONIC_LOG_DEBUG("Unknown UISize mode");
				return 0.0f;
			}
		}

		float toAbsoluteHeight(Scene* scene, EntityID parent, const UISize& height)
		{
			if (height.mode == UISize::Mode::Absolute)
				return height.value;
			else if (height.mode == UISize::Mode::RelativeToWindow)
				return height.value * Window::getHeight();
			else if (height.mode == UISize::Mode::RelativeToEntity)
			{
				SONIC_LOG_DEBUG_ASSERT(parent != 0, "UIComponent: Must spectify a parent entity if RelativeToEntity is the size mode");
				UIComponent* parentComponent = scene->GetComponent<UIComponent>(parent);
				return height.value * toAbsoluteHeight(scene, parentComponent->parent, parentComponent->height);
			}
			else
			{
				SONIC_LOG_DEBUG("Unknown UISize mode");
				return 0.0f;
			}
		}

	}

}
