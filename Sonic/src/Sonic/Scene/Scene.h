#pragma once
#include "Sonic/Util/GenericContainer.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Renderer/Camera2D.h"
#include "Sonic/Window/Window.h"
#include "Sonic/UI/UIComponents.h"
#include "EntityID.h"
#include "ComponentPool.h"
#include "Behaviour.h"
#include "BehaviourPool.h"

namespace Sonic {

	template<typename Component>
	class PairView;

	template<typename Component>
	class EntityView;

	template<typename Component>
	class ComponentView;

	template<typename Component1, typename Component2>
	class GroupView;

	class Entity;

	class Scene : public EventDispatcher
	{
	protected:
		template<typename Component>
		struct ComponentAddedEvent
		{
			EntityID entity;
		};

		template<typename Component>
		struct ComponentRemovedEvent
		{
			EntityID entity;
		};

	protected:
		Scene();

	private:
		void Init();
		void Update(float deltaTime);
		void Draw();
		void Render();

		void UpdatePools();
		void UpdateComponents(float deltaTime);
		void UpdateBehaviours(float deltaTime);

		void OnMouseButtonPressed(const MouseButtonPressedEvent& e);
		void OnMouseButtonReleased(const MouseButtonReleasedEvent& e);
		void OnMouseMoved(const MouseMovedEvent& e);
		void OnMouseDragged(const MouseDraggedEvent& e);

		void OnUIComponentAdded(const ComponentAddedEvent<UIComponent>& e);
		void OnUIComponentRemoved(const ComponentRemovedEvent<UIComponent>& e);

		void RenderUIEntity(EntityID entity, Sprite* sprite, Color* color, float x, float y, float zIndex, float width, float height);

	protected:
		virtual void Load() = 0;
		virtual void OnInit() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void PollCollisionEvents() {}

	public:
		Entity AddEntity();
		void RemoveEntity(EntityID entity);
		Entity ToEntity(EntityID entity);

		template<typename Component, typename... Args>
		void AddComponent(EntityID entity, Args&&... args)
		{
			ComponentPool<Component>* pool = GenericContainer::Get<ComponentPool<Component>, BaseComponentPool>(this);
			pool->AddComponent(entity, std::forward<Args>(args)...);
			DispatchEvent(ComponentAddedEvent<Component>{ entity });
		}

		template<typename DerivedBehaviour, typename... Args>
		void AddBehaviour(EntityID entity, Args&&... args)
		{
			GenericContainer::Get<BehaviourPool<DerivedBehaviour>, BaseBehaviourPool>(this, this)->AddBehaviour(entity, std::forward<Args>(args)...);
		}

		template<typename Component>
		bool HasComponent(EntityID entity)
		{
			return GenericContainer::Get<ComponentPool<Component>, BaseComponentPool>(this)->HasEntity(entity);
		}

		template<typename Component>
		Component* GetComponent(EntityID entity)
		{
			return GenericContainer::Get<ComponentPool<Component>, BaseComponentPool>(this)->GetComponent(entity);
		}

		template<typename Component>
		void RemoveComponent(EntityID entity)
		{
			GenericContainer::Get<ComponentPool<Component>, BaseComponentPool>(this)->RemoveEntity(entity);
			DispatchEvent(ComponentRemovedEvent<Component>{ entity });
		}

		template<typename DerivedBehaviour>
		void RemoveBehaviour(EntityID entity)
		{
			GenericContainer::Get<BehaviourPool<DerivedBehaviour>, BaseBehaviourPool>(this, this)->RemoveEntity(entity);
		}

		template<typename Component>
		EntityView<Component>& ViewEntities()
		{
			return *GenericContainer::Get<EntityView<Component>>(this, this);
		}

		template<typename Component>
		ComponentView<Component>& ViewComponents()
		{
			return *GenericContainer::Get<ComponentView<Component>>(this, this);
		}

		template<typename Component>
		PairView<Component>& View()
		{
			return *GenericContainer::Get<PairView<Component>>(this, this);
		}

		template<typename Component1, typename Component2>
		GroupView<Component1, Component2>& Group()
		{
			return *GenericContainer::Get<GroupView<Component1, Component2>>(this, this);
		}

	private:
		Camera2D m_Camera;

		EntityID m_ResizingUIEntity = 0;

		friend class App;
		friend class Entity;
		template<typename, typename> friend class GroupView;
	};

}

