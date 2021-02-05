#pragma once
#include "Sonic/Scene/EntityID.h"

namespace Sonic {

	class Scene;

	struct MouseButtonReleasedEvent;
	struct MouseMovedEvent;
	struct WindowResizedEvent;

	struct UIComponent;
	struct UIHoverComponent;
	struct UITextComponent;
	struct UIRendererComponent;
	struct UIResizableComponent;
	struct UIMovableComponent;

	template<typename Component>
	struct ComponentAddedEvent;

	template<typename Component>
	struct ComponentRemovedEvent;

	struct UISize;

	class SceneUIHandler
	{
		SceneUIHandler() = delete;
		SceneUIHandler(const SceneUIHandler& other) = delete;
		SceneUIHandler(Scene* scene);

		void Update(float deltaSeconds);

		void OnMouseButtonReleased(const MouseButtonReleasedEvent& e);
		void OnMouseMoved(const MouseMovedEvent& e);
		void OnWindowResized(const WindowResizedEvent& e);

		void OnComponentAdded(const ComponentAddedEvent<UIComponent>& e);
		void OnComponentAdded(const ComponentAddedEvent<UIHoverComponent>& e);
		void OnComponentAdded(const ComponentAddedEvent<UITextComponent>& e);
		void OnComponentAdded(const ComponentAddedEvent<UIRendererComponent>& e);
		void OnComponentRemoved(const ComponentRemovedEvent<UIComponent>& e);

		float CalculateAbsoluteX(const UISize& x, UIComponent* parent);
		float CalculateAbsoluteY(const UISize& y, UIComponent* parent);
		float CalculateAbsoluteWidth(const UISize& width, UIComponent* parent);
		float CalculateAbsoluteHeight(const UISize& height, UIComponent* parent);

		void UpdateUIResizableComponentMouseButtonDown(EntityID entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIResizableComponentMouseButtonUp(EntityID entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIMovableComponentMouseButtonDown(UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIMovableComponentMouseButtonUp(UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e);

		void ResizeChilds(UIComponent* component);

		Scene* m_Scene;

		friend class Scene;
	};

}
