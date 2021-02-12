#pragma once
#include <vector>
#include <unordered_map>
#include "Sonic/Scene/ECS/Entity.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/Components/UIComponents.h"
#include "UISize.h"

namespace Sonic {

	class Scene;

	class SceneUIHandler
	{
	public:
		void SetX(Entity entity, UIComponent* component, float value, UIComponent* parent, bool resizeChilds = true);
		void SetY(Entity entity, UIComponent* component, float value, UIComponent* parent, bool resizeChilds = true);
		void SetWidth(Entity entity, UIComponent* component, float value, UIComponent* parent);
		void SetHeight(Entity entity, UIComponent* component, float value, UIComponent* paren);

		void SetX(Entity entity, UIComponent* component, float value, bool resizeChilds = true);
		void SetY(Entity entity, UIComponent* component, float value, bool resizeChilds = true);
		void SetWidth(Entity entity, UIComponent* component, float value);
		void SetHeight(Entity entity, UIComponent* component, float value);
	
	private:
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
		void OnComponentAdded(const ComponentAddedEvent<UIPositionConstraintsComponent>& e);
		void OnComponentAdded(const ComponentAddedEvent<UISizeConstraintsComponent>& e);
		void OnComponentRemoved(const ComponentRemovedEvent<UIComponent>& e);

		void UpdateUIResizableComponentMouseButtonDown(Entity entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIResizableComponentMouseButtonUp(Entity entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIMovableComponentMouseButtonDown(Entity entity, UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIMovableComponentMouseButtonUp(UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e);

		void ResizeChildsX(Entity parent, UIComponent* c);
		void ResizeChildsY(Entity parent, UIComponent* c);
		void ResizeChildsWidth(Entity parent, UIComponent* c);
		void ResizeChildsHeight(Entity parent, UIComponent* c);

		void MarkDirty(UIComponent* component);

		void FitPosition(Entity entity, UIComponent* c, UIPositionConstraintsComponent* constraints);
		void FitSize(Entity entity, UIComponent* c, UISizeConstraintsComponent* constraints);

		void Destroy();

		Scene* m_Scene;
		std::unordered_map<Entity, std::vector<Entity>> m_ChildRegistry;

		friend class Scene;
	};

}
