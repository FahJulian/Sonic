#pragma once

namespace Sonic {

	class Scene;

	struct MouseButtonReleasedEvent;
	struct MouseButtonPressedEvent;
	struct MouseMovedEvent;
	struct WindowResizedEvent;

	struct UIComponent;
	struct UIHoverComponent;
	struct UITextComponent;
	struct UIRendererComponent;
	struct UIResizableComponent;

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

		void OnMouseButtonPressed(const MouseButtonPressedEvent& e);
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

		void ResizeChilds(UIComponent* component);

		Scene* m_Scene;

		friend class Scene;
	};

}
