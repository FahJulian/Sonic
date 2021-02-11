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

		template<typename Component>
		AnonymousComponentPool* GetComponentPool()
		{
			ComponentType type = getComponentType<Component>();

			SONIC_LOG_DEBUG_ASSERT(type <= m_ComponentPools.size(), "Error: Cant add Component Pool because on in between is missing.");

			if (type == m_ComponentPools.size())
				m_ComponentPools.push_back(new ComponentPool<Component>());
			
			return m_ComponentPools.at(type);
		}

		void DeactivateEntity(Entity entity);

		void ReactivateEntity(Entity entity);

		void RemoveEntity(Entity entity);

		template<typename Component, typename... Args>
		void AddComponent(Entity entity, Args&&... args)
		{
			AnonymousComponentPool* pool = GetComponentPool<Component>();
			pool->AddComponent<Component>(entity, std::forward<Args>(args)...);
			m_ComponentMap[entity].push_back(getComponentType<Component>());
		}

		template<typename Component>
		bool HasComponent(Entity entity)
		{
			AnonymousComponentPool* pool = GetComponentPool<Component>();
			return pool->HasEntity(entity);
		}

		template<typename Component>
		Component* GetComponent(Entity e)
		{
			AnonymousComponentPool* pool = GetComponentPool<Component>();
			return pool->GetComponent<Component>(e);
		}

		template<typename Component>
		void RemoveComponent(Entity entity)
		{
			AnonymousComponentPool* pool = GetComponentPool<Component>();
			pool->RemoveEntity(entity);
			m_ComponentMap[entity].erase(std::remove(m_ComponentMap[entity].begin(), m_ComponentMap[entity].end(), getComponentType<Component>()));
		}

		~ComponentRegistry()
		{
			for (AnonymousComponentPool* pool : m_ComponentPools)
				delete pool;
		}

		std::vector<AnonymousComponentPool*> m_ComponentPools;
		std::unordered_map<Entity, std::vector<ComponentType>> m_ComponentMap;
	};

}
