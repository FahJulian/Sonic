#pragma once
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/ECS/Entity.h"
#include "Sonic/Scene/Components/UIComponents.h"

namespace Sonic {

	class Scene;

	class SceneUIHandler
	{
	private:
		SceneUIHandler(Scene* scene);
		SceneUIHandler() = delete;
		SceneUIHandler(const SceneUIHandler& other) = delete;

		void Init();

		void OnMouseButtonPressed(const MouseButtonPressedEvent& e);
		void OnMouseButtonReleased(const MouseButtonReleasedEvent& e);
		void OnMouseMoved(const MouseMovedEvent& e);
		void OnWindowResized(const WindowResizedEvent& e);

		void OnComponentAdded(const ComponentAddedEvent<UIComponent>& e);
		void OnComponentAdded(const ComponentAddedEvent<UIHoverComponent>& e);
		void OnComponentAdded(const ComponentAddedEvent<UITextComponent>& e);
		void OnComponentAdded(const ComponentAddedEvent<UIRendererComponent>& e);
		void OnComponentAdded(const ComponentAddedEvent<UIPositionConstraintsComponent>& e);
		void OnComponentAdded(const ComponentAddedEvent<UISizeConstraintsComponent>& e);
		void OnComponentRemoved(const ComponentRemovedEvent<UIComponent>& e);
		void OnComponentRemoved(const ComponentRemovedEvent<UIRendererComponent>& e);
		void OnComponentRemoved(const ComponentRemovedEvent<UITextComponent>& e);
		void OnEntityDeactivated(const EntityDeactivatedEvent& e);
		void OnEntityReactivated(const EntityReactivatedEvent& e);
		void OnComponentDeactivated(const ComponentDeactivatedEvent<UIComponent>& e);
		void OnComponentDeactivated(const ComponentDeactivatedEvent<UITextComponent>& e);
		void OnComponentDeactivated(const ComponentDeactivatedEvent<UIRendererComponent>& e);
		void OnComponentReactivated(const ComponentReactivatedEvent<UIComponent>& e);
		void OnComponentReactivated(const ComponentReactivatedEvent<UITextComponent>& e);
		void OnComponentReactivated(const ComponentReactivatedEvent<UIRendererComponent>& e);
		void OnComponentReactivated(const ComponentReactivatedEvent<UIPositionConstraintsComponent>& e);
		void OnComponentReactivated(const ComponentReactivatedEvent<UISizeConstraintsComponent>& e);

		void UpdateUIResizableComponentMouseButtonDown(Entity entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIResizableComponentMouseButtonUp(Entity entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIMovableComponentMouseButtonDown(Entity entity, UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIMovableComponentMouseButtonUp(UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e);

		Scene* m_Scene;

		friend class Scene;
	};

}
