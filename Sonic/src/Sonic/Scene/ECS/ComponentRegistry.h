#pragma once
#include <unordered_map>
#include <vector>
#include "Sonic/Debug/Log/Log.h"
#include "Entity.h"
#include "ComponentType.h"
#include "ComponentPool.h"

namespace Sonic {

	class ComponentRegistry
	{
	public:
		ComponentRegistry() = default;

		void DeactivateEntity(Entity entity);

		void ReactivateEntity(Entity entity);

		void RemoveEntity(Entity entity);

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
		void RemoveComponent(Entity entity)
		{
			ComponentPool* pool = GetComponentPool(ComponentPool::getComponentType<Component>());
			pool->RemoveEntity(entity);
			m_ComponentMap[entity].erase(std::remove(m_ComponentMap[entity].begin(), m_ComponentMap[entity].end(), ComponentPool::getComponentType<Component>()));
		}

	private:
		std::vector<ComponentPool*> m_ComponentPools;
		std::unordered_map<Entity, std::vector<ComponentType>> m_ComponentMap;

		template<typename> friend class EntityView;
		template<typename> friend class ComponentView;
		template<typename> friend class PairView;
		template<typename, typename> friend class GroupView;
	};

}
