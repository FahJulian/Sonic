#pragma once
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Renderer/Camera2D.h"
#include "Sonic/Window/Window.h"
#include "EntityID.h"
#include "ComponentPool.h"
#include "ComponentView.h"
#include "Behaviour.h"
#include "BehaviourPool.h"

namespace Sonic {

	class Entity;

	class Scene : public EventDispatcher
	{
	protected:
		Scene();

	private:
		void Init();
		void Update(float deltaTime);
		void Render();

		void UpdatePools();

	protected:
		virtual void Load() = 0;
		virtual void OnInit() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void CheckCollisions() {}

	public:
		Entity AddEntity();
		void RemoveEntity(EntityID entity);
		Entity ToEntity(EntityID entity);

		template<typename Component, typename... Args>
		void AddComponent(EntityID entity, Args&&... args)
		{
			GetComponentPool<Component>()->AddComponent(entity, std::forward<Args>(args)...);
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
		ComponentView<Component> View()
		{
			return ComponentView<Component>(GetComponentPool<Component>());
		}

	private:
		template<typename Component>
		ComponentPool<Component>* GetComponentPool()
		{
			static ComponentPool<Component>* pool = CreateComponentPool<Component>();
			return pool;
		}

		template<typename Component>
		ComponentPool<Component>* CreateComponentPool()
		{
			static ComponentPool<Component> pool;
			m_ComponentPools.push_back(&pool);
			return &pool;
		}

		template<typename DerivedBehaviour>
		BehaviourPool<DerivedBehaviour>* GetBehaviourPool()
		{
			static BehaviourPool<DerivedBehaviour>* pool = CreateBehaviourPool<DerivedBehaviour>();
			return pool;
		}

		template<typename DerivedBehaviour>
		BehaviourPool<DerivedBehaviour>* CreateBehaviourPool()
		{
			static BehaviourPool<DerivedBehaviour> pool(this);
			m_BehaviourPools.push_back(static_cast<BaseBehaviourPool*>(&pool));
			return &pool;
		}

		std::vector<BaseComponentPool*> m_ComponentPools;
		std::vector<BaseBehaviourPool*> m_BehaviourPools;

		Camera2D m_Camera;

		friend class App;
		friend class Entity;
	};

}

