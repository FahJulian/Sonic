#pragma once
#include <vector>
#include <unordered_map>
#include "Sonic/Scene/EntityID.h"
#include "Sonic/Event/Events.h"
#include "UIComponents.h"
#include "UISize.h"

namespace Sonic {

	class Scene;

	class SceneUIHandler
	{
	public:
		void SetX(EntityID entity, UIComponent* component, float value, UIComponent* parent = nullptr, bool resizeChilds = true);
		void SetY(EntityID entity, UIComponent* component, float value, UIComponent* parent = nullptr, bool resizeChilds = true);
		void SetWidth(EntityID entity, UIComponent* component, float value, UIComponent* parent = nullptr);
		void SetHeight(EntityID entity, UIComponent* component, float value, UIComponent* parent = nullptr);
	
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

		void UpdateUIResizableComponentMouseButtonDown(EntityID entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIResizableComponentMouseButtonUp(EntityID entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIMovableComponentMouseButtonDown(EntityID entity, UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e);
		void UpdateUIMovableComponentMouseButtonUp(UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e);

		void ResizeChildsX(EntityID parent, UIComponent* c);
		void ResizeChildsY(EntityID parent, UIComponent* c);
		void ResizeChildsWidth(EntityID parent, UIComponent* c);
		void ResizeChildsHeight(EntityID parent, UIComponent* c);

		void MarkDirty(UIComponent* component);

		void FitPosition(EntityID entity, UIComponent* c, UIPositionConstraintsComponent* constraints);
		void FitSize(EntityID entity, UIComponent* c, UISizeConstraintsComponent* constraints);

		Scene* m_Scene;
		std::unordered_map<EntityID, std::vector<EntityID>> m_ChildRegistry;

		friend class Scene;
	};

}
