#pragma once
#include <vector>
#include "Sonic/Log/Log.h"
#include "Entity.h"

namespace Sonic {

	class AnonymousComponentPool
	{
		struct GroupViewInfo
		{
			AnonymousComponentPool* otherPool;
			std::vector<Entity>* entities;
			std::vector<size_t*>* activeIteratorIndices;

			GroupViewInfo(AnonymousComponentPool* otherPool, std::vector<Entity>* entities, std::vector<size_t*>* activeIteratorIndices)
				: otherPool(otherPool), entities(entities), activeIteratorIndices(activeIteratorIndices)
			{
			}
		};

		AnonymousComponentPool()
			: m_Size(0), m_Capacity(0), m_Cursor(0), m_Data(nullptr), m_Entities(nullptr),
			m_InactiveSize(0), m_InactiveCapacity(0), m_InactiveCursor(0), m_InactiveData(nullptr), m_InactiveEntities(nullptr)
		{
		}

		AnonymousComponentPool(const AnonymousComponentPool& other) = delete;
		AnonymousComponentPool& operator=(const AnonymousComponentPool& other) = delete;

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

			for (GroupViewInfo groupView : m_GroupViews)
				if (groupView.otherPool->HasEntity(entity))
					groupView.entities->push_back(entity);

			m_Size++;
		}

		void DeactivateEntity(Entity entity, size_t componentSize);

		void ReactivateEntity(Entity entity, size_t componentSize);

		bool HasEntity(Entity entity);

		virtual void RemoveEntity(Entity entity) = 0;

		size_t IndexOf(Entity entity);

		size_t IndexOfInactive(Entity entity);

		void IncreaseSize(size_t componentSize);

		void IncreaseInactiveSize(size_t componentSize);

		virtual ~AnonymousComponentPool() {};

		size_t m_Size;
		size_t m_Capacity;
		size_t m_Cursor;
		uint8_t* m_Data;
		Entity* m_Entities;

		size_t m_InactiveSize;
		size_t m_InactiveCapacity;
		size_t m_InactiveCursor;
		uint8_t* m_InactiveData;
		Entity* m_InactiveEntities;

		std::vector<size_t*> m_ActiveIteratorIndices;
		std::vector<GroupViewInfo> m_GroupViews;

		static const size_t NOT_FOUND = std::numeric_limits<size_t>::max();

		template<typename> friend class EntityView;
		template<typename> friend class ComponentView;
		template<typename> friend class PairView;
		template<typename, typename> friend class GroupView;
		template<typename> friend class ComponentPool;
		friend class ComponentRegistry;
	};

	template<typename Component>
	class ComponentPool : public AnonymousComponentPool
	{
	public:
		ComponentPool()
			: AnonymousComponentPool()
		{
		}

		ComponentPool(const ComponentPool<Component>& other) = delete;
		ComponentPool<Component>& operator=(const ComponentPool<Component>& other) = delete;

		void RemoveEntity(Entity entity) override
		{
			size_t index = IndexOf(entity);

			SONIC_LOG_DEBUG_ASSERT(index != NOT_FOUND, "Cant remove Entity from ComponentPool: Pool does not contain Entity");

			Component* data = reinterpret_cast<Component*>(m_Data);
			data[index].~Component();

			std::copy(data + index + 1, data + m_Size, data + index);
			std::copy(m_Entities + index + 1, m_Entities + m_Size, m_Entities + index);

			for (size_t* iteratorIndex : m_ActiveIteratorIndices)
			{
				if (*iteratorIndex >= index)
					(*iteratorIndex)--;
			}

			for (GroupViewInfo groupView : m_GroupViews)
			{
				for (size_t i = 0, size = groupView.entities->size(); i < size; i++)
				{
					if (groupView.entities->at(i) == entity)
					{
						groupView.entities->erase(groupView.entities->begin() + i);
						for (size_t* iteratorIndex : *groupView.activeIteratorIndices)
							if (*iteratorIndex >= i)
								--(*iteratorIndex);
						return;
					}
				}
			}

			m_Size--;
		}

		~ComponentPool() override
		{
			Component* data = reinterpret_cast<Component*>(m_Data);
			for (size_t i = 0; i < m_Size; i++)
				data[i].~Component();

			Component* inactiveData = reinterpret_cast<Component*>(m_InactiveData);
			for (size_t i = 0; i < m_InactiveSize; i++)
				inactiveData[i].~Component();
		}
	};

}