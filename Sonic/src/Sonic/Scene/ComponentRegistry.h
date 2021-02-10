#pragma once
#include <unordered_map>
#include <vector>
#include "Entity.h"
#include "ComponentType.h"
#include "ComponentPool.h"

namespace Sonic {

	class ComponentRegistry
	{
	public:
		ComponentRegistry() = default;

		ComponentPool* GetComponentPool(ComponentType type);

		bool HasEntity(Entity entity);

		void TransferEntity(Entity entity, ComponentRegistry * other);

		void RemoveEntity(Entity entity);

		~ComponentRegistry();

		template<typename Component, typename... Args>
		void AddComponent(Entity entity, Args&&... args)
		{
			ComponentType type = getComponentType<Component>();
			ComponentPool* pool = GetComponentPool(type);
			pool->AddComponent<Component>(entity, std::forward<Args>(args)...);
			m_ComponentMap[entity].push_back(type);
		}

		template<typename Component>
		bool HasComponent(Entity entity)
		{
			ComponentPool* pool = GetComponentPool(getComponentType<Component>());
			return pool->HasEntity(entity);
		}

		template<typename Component>
		Component* GetComponent(Entity e)
		{
			ComponentPool* pool = GetComponentPool(getComponentType<Component>());
			return pool->GetComponent<Component>(e);
		}

		template<typename Component>
		void RemoveComponent(Entity entity)
		{
			ComponentType type = getComponentType<Component>();
			ComponentPool* pool = GetComponentPool(type);
			pool->RemoveEntity(entity, sizeof(Component));
			m_ComponentMap[entity].erase(std::remove(m_ComponentMap[entity].begin(), m_ComponentMap[entity].end(), type));
		}

		std::vector<ComponentPool*> m_ComponentPools;
		std::unordered_map<Entity, std::vector<ComponentType>> m_ComponentMap;
	};

}
