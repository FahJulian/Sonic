#pragma once
#include <unordered_map>
#include <vector>
#include "Sonic/Debug/Log/Log.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events/ECSEvents.h"
#include "Views.h"
#include "Entity.h"
#include "EntityGroup.h"
#include "ComponentType.h"
#include "ComponentPool.h"

namespace Sonic {

	class ComponentRegistry
	{
	public:
		ComponentRegistry() = default;

		Entity AddEntity();
		Entity AddEntity(EntityGroup group);

		EntityGroup AddEntityGroup();
		std::vector<Entity>* GetGroup(EntityGroup group);
		void AddToGroup(EntityGroup group, Entity entity);

		void DeactivateEntity(Entity entity);
		void ReactivateEntity(Entity entity);
		void RemoveEntity(Entity entity);

		void DeactivateEntities(EntityGroup group);
		void ReactivateEntities(EntityGroup group);

		void Destroy();

	private:
		ComponentPool* GetComponentPool(ComponentType type);

	public:
		template<typename Component, typename... Args>
		void AddComponent(Entity entity, Args&&... args)
		{
			ComponentPool* pool = GetComponentPool(ComponentPool::getComponentType<Component>());
			pool->AddComponent<Component>(entity, std::forward<Args>(args)...);
			m_ComponentMap[entity].push_back(ComponentPool::getComponentType<Component>());
			EventDispatcher::dispatch(ComponentAddedEvent<Component>(entity));
		}

		template<typename Component>
		bool HasComponent(Entity entity)
		{
			ComponentPool* pool = GetComponentPool(ComponentPool::getComponentType<Component>());
			return pool->HasEntity(entity);
		}

		template<typename Component>
		Component* GetComponent(Entity e)
		{
			ComponentPool* pool = GetComponentPool(ComponentPool::getComponentType<Component>());
			return pool->GetComponent<Component>(e);
		}

		template<typename Component>
		void DeactivateComponent(Entity entity)
		{
			EventDispatcher::dispatch(ComponentDeactivatedEvent<Component>(entity));
			ComponentPool* pool = GetComponentPool(ComponentPool::getComponentType<Component>());
			pool->DeactivateEntity(entity);
			m_ComponentMap[entity].erase(std::remove(m_ComponentMap[entity].begin(), m_ComponentMap[entity].end(), ComponentPool::getComponentType<Component>()));
		}

		template<typename Component>
		void ReactivateComponent(Entity entity)
		{
			ComponentPool* pool = GetComponentPool(ComponentPool::getComponentType<Component>());
			pool->ReactivateEntity(entity);
			m_ComponentMap[entity].push_back(ComponentPool::getComponentType<Component>());
			EventDispatcher::dispatch(ComponentReactivatedEvent<Component>(entity));
		}

		template<typename Component>
		void RemoveComponent(Entity entity)
		{
			ComponentPool* pool = GetComponentPool(ComponentPool::getComponentType<Component>());
			pool->RemoveEntity(entity);
			m_ComponentMap[entity].erase(std::remove(m_ComponentMap[entity].begin(), m_ComponentMap[entity].end(), ComponentPool::getComponentType<Component>()));
			EventDispatcher::dispatch(ComponentRemovedEvent<Component>(entity));
		}

		template<typename Component>
		EntityView<Component> ViewEntities()
		{
			return EntityView<Component>(GetComponentPool(ComponentPool::getComponentType<Component>()));
		}

		template<typename Component>
		ComponentView<Component> ViewComponents()
		{
			return ComponentView<Component>(GetComponentPool(ComponentPool::getComponentType<Component>()));
		}

		template<typename Component>
		PairView<Component> View()
		{
			return PairView<Component>(GetComponentPool(ComponentPool::getComponentType<Component>()));
		}

		template<typename Component1, typename Component2>
		GroupView<Component1, Component2>& Group()
		{
			static std::unordered_map<ComponentRegistry*, GroupView<Component1, Component2>*> groups;

			auto it = groups.find(this);
			if (it == groups.end())
			{
				groups.emplace(this, new GroupView<Component1, Component2>(GetComponentPool(ComponentPool::getComponentType<Component1>()), GetComponentPool(ComponentPool::getComponentType<Component2>())));
				it = --(groups.end());
			}

			return *it->second;
		}

	private:
		std::vector<ComponentPool*> m_ComponentPools;
		std::unordered_map<Entity, std::vector<ComponentType>> m_ComponentMap;

		Entity m_NextEntity = 1;
		std::unordered_map<EntityGroup, std::vector<Entity>> m_EntityGroups;
	};

}
