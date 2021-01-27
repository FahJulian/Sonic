#pragma once
#include "Sonic/Graphics/Graphics2D/Sprite.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Renderer/Camera2D.h"
#include "Sonic/Window/Window.h"
#include "Sonic/UI/UIComponents.h"
#include "EntityID.h"
#include "ComponentPool.h"
#include "EntityView.h"
#include "ComponentView.h"
#include "PairView.h"
#include "GroupView.h"
#include "Behaviour.h"
#include "BehaviourPool.h"

namespace Sonic {


	class Entity;

	class Scene : public EventDispatcher
	{
	public:
		template<typename Component>
		struct ComponentAddedEvent
		{
		};

		template<typename Component>
		struct ComponentRemovedEvent
		{
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
			ComponentPool<Component>* pool = GetComponentPool<Component>();
			pool->AddComponent(entity, std::forward<Args>(args)...);
			DispatchEvent(ComponentAddedEvent<Component>());
		}

		template<typename DerivedBehaviour, typename... Args>
		void AddBehaviour(EntityID entity, Args&&... args)
		{
			GetBehaviourPool<DerivedBehaviour>()->AddBehaviour(entity, std::forward<Args>(args)...);
		}

		template<typename Component>
		bool HasComponent(EntityID entity)
		{
			return GetComponentPool<Component>()->HasEntity(entity);
		}

		template<typename Component>
		Component* GetComponent(EntityID entity)
		{
			return GetComponentPool<Component>()->GetComponent(entity);
		}

		template<typename Component>
		void RemoveComponent(EntityID entity)
		{
			GetComponentPool<Component>()->RemoveEntity(entity);
		}

		template<typename DerivedBehaviour>
		void RemoveBehaviour(EntityID entity)
		{
			GetBehaviourPool<DerivedBehaviour>()->RemoveEntity(entity);
		}

		template<typename Component>
		EntityView<Component> ViewEntities()
		{
			return EntityView<Component>(GetComponentPool<Component>());
		}

		template<typename Component>
		ComponentView<Component> ViewComponents()
		{
			return ComponentView<Component>(GetComponentPool<Component>());
		}

		template<typename Component>
		PairView<Component> View()
		{
			return PairView<Component>(GetComponentPool<Component>());
		}

		template<typename Component1, typename Component2>
		GroupView<Component1, Component2> Group()
		{
			return GroupView<Component1, Component2>(GetComponentPool<Component1>(), GetComponentPool<Component2>());
		}

	private:
		template<typename Component>
		ComponentPool<Component>* GetComponentPool()
		{
			static std::unordered_map<Scene*, ComponentPool<Component>*> pools;

			if (pools.find(this) == pools.end())
			{
				pools.emplace(this, new ComponentPool<Component>());
				m_ComponentPools.push_back(pools[this]);
			}

			return pools[this];
		}

		template<typename DerivedBehaviour>
		BehaviourPool<DerivedBehaviour>* GetBehaviourPool()
		{
			static std::unordered_map<Scene*, BehaviourPool<DerivedBehaviour>*> pools;

			if (pools.find(this) == pools.end())
			{
				pools.emplace(this, new BehaviourPool<DerivedBehaviour>(this));
				m_BehaviourPools.push_back(pools[this]);
			}

			return pools[this];
		}

		std::vector<BaseComponentPool*> m_ComponentPools;
		std::vector<BaseBehaviourPool*> m_BehaviourPools;

		Camera2D m_Camera;

		EntityID m_ResizingUIEntity = 0;

		friend class App;
		friend class Entity;
	};

}

