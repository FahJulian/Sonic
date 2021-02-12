#pragma once
#include <unordered_map>
#include <vector>
#include "Sonic/Log/Log.h"
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

		template<typename Component>
		ComponentPool* GetComponentPool()
		{
			ComponentType type = ComponentPool::getComponentType<Component>();

			while (type >= m_ComponentPools.size())
				m_ComponentPools.push_back(new ComponentPool(static_cast<ComponentType>(m_ComponentPools.size())));

			return m_ComponentPools.at(type);
		}

		template<typename Component, typename... Args>
		void AddComponent(Entity entity, Args&&... args)
		{
			ComponentPool* pool = GetComponentPool<Component>();
			pool->AddComponent<Component>(entity, std::forward<Args>(args)...);
			m_ComponentMap[entity].push_back(ComponentPool::getComponentType<Component>());
		}

		template<typename Component>
		bool HasComponent(Entity entity)
		{
			ComponentPool* pool = GetComponentPool<Component>();
			return pool->HasEntity(entity);
		}

		template<typename Component>
		Component* GetComponent(Entity e)
		{
			ComponentPool* pool = GetComponentPool<Component>();
			return pool->GetComponent<Component>(e);
		}

		template<typename Component>
		void RemoveComponent(Entity entity)
		{
			ComponentPool* pool = GetComponentPool<Component>();
			pool->RemoveEntity(entity);
			m_ComponentMap[entity].erase(std::remove(m_ComponentMap[entity].begin(), m_ComponentMap[entity].end(), ComponentPool::getComponentType<Component>()));
		}

		std::vector<ComponentPool*> m_ComponentPools;
		std::unordered_map<Entity, std::vector<ComponentType>> m_ComponentMap;
	};

}
