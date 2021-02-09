#pragma once
#include <vector>
#include "Sonic/Log/Log.h"
#include "Entity.h"

namespace Sonic {

	class ComponentPool
	{
		ComponentPool()
			: m_Size(0), m_Capacity(0), m_Cursor(0), m_Data(nullptr), m_Entities(nullptr)
		{
		}

		template<typename Component>
		Component* GetComponent(Entity e)
		{
			size_t index = IndexOf(e);

			SONIC_LOG_DEBUG_ASSERT(index != NOT_FOUND, "Cant get Component from ComponentPool: Pool does not contain Entity");

			return reinterpret_cast<Component*>(m_Data) + index;
		}

		template<typename Component, typename... Args>
		void AddComponent(Entity entity, Args&&... args)
		{
			if (m_Size == m_Capacity)
				IncreaseSize(sizeof(Component));

#pragma warning(disable:4244)
			new(reinterpret_cast<Component*>(m_Data + m_Size * sizeof(Component))) Component(std::forward<Args>(args)...);
#pragma warning(default:4244)

			m_Entities[m_Size] = entity;

			m_Size++;
		}

		uint8_t* GetComponent(Entity entity, size_t componentSize);

		void AddComponent(Entity entity, uint8_t* data, size_t componentSize);

		void TransferEntity(Entity entity, ComponentPool* other, size_t componentSize);

		bool HasEntity(Entity entity);

		void RemoveEntity(Entity entity, size_t componentSize);

		size_t IndexOf(Entity entity);

		void IncreaseSize(size_t componentSize);

		void Destroy();

		size_t m_Size;
		size_t m_Capacity;
		size_t m_Cursor;
		uint8_t* m_Data;
		Entity* m_Entities;
		std::vector<size_t*> m_ActiveIteratorIndices;

		static const size_t NOT_FOUND = std::numeric_limits<size_t>::max();

		friend class EntityView;
		template<typename> friend class ComponentView;
		template<typename> friend class PairView;
		friend class ComponentRegistry;
	};

}