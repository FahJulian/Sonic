#pragma once
#include <vector>
#include <functional>
#include "Sonic/Debug/Log/Log.h"
#include "ComponentType.h"
#include "Entity.h"

namespace Sonic {


	class ComponentPool
	{
	private:
		struct Data
		{
			size_t size = 0;
			size_t capacity = 0;
			size_t cursor = 0;
			uint8_t* data = nullptr;
			Entity* entities = nullptr;
		};

		struct GroupViewInfo
		{
			ComponentPool* otherPool;
			std::vector<Entity>* entities;
			std::vector<size_t*>* activeIteratorIndices;

			GroupViewInfo(ComponentPool* otherPool, std::vector<Entity>* entities, std::vector<size_t*>* activeIteratorIndices)
				: otherPool(otherPool), entities(entities), activeIteratorIndices(activeIteratorIndices)
			{
			}
		};

		ComponentPool(ComponentType type)
			: m_Type(type), m_ComponentSize(s_ComponentSizes.at(type)), m_ComponentDestructor(s_ComponentDestructors.at(type))
		{
		}

		ComponentPool(const ComponentPool& other) = delete;
		ComponentPool& operator=(const ComponentPool& other) = delete;

		void DeactivateEntity(Entity entity);
		void ReactivateEntity(Entity entity);
		bool HasEntity(Entity entity);
		void RemoveEntity(Entity entity);

		size_t IndexOf(Entity entity, Data* data);
		void IncreaseSize(Data* data);
		void MoveEntity(Data* src, Data* dst, size_t srcIndex);
		void NotifyEntityRemoved(Entity entity, size_t index);
		void NotifyEntityAdded(Entity entity);

		~ComponentPool();

		template<typename Component>
		Component* GetComponent(Entity e)
		{
			size_t index = IndexOf(e, &m_Active);

			SONIC_DEBUG_ASSERT(index != NOT_FOUND, "Cant get Component from ComponentPool: Pool does not contain Entity");

			return reinterpret_cast<Component*>(m_Active.data) + index;
		}

		template<typename Component, typename... Args>
		void AddComponent(Entity entity, Args&&... args)
		{
			if (m_Active.size == m_Active.capacity)
				IncreaseSize(&m_Active);

#pragma warning(disable:4244)
			new(reinterpret_cast<Component*>(m_Active.data + m_Active.size * m_ComponentSize)) Component(std::forward<Args>(args)...);
#pragma warning(default:4244)

			m_Active.entities[m_Active.size] = entity;

			NotifyEntityAdded(entity);

			m_Active.size++;
		}

		template<typename Component>
		static ComponentType registerComponentType()
		{
			s_ComponentSizes.push_back(sizeof(Component));
			s_ComponentDestructors.push_back([](uint8_t* data) {
				(*reinterpret_cast<Component*>(data)).~Component();
			});

			return s_NextComponentType++;
		}

		template<typename Component>
		static ComponentType getComponentType()
		{
			static ComponentType type = registerComponentType<Component>();
			return type;
		}

		Data m_Active;
		Data m_Inactive;

		ComponentType m_Type;
		size_t m_ComponentSize;
		std::function<void(uint8_t*)> m_ComponentDestructor;

		std::vector<size_t*> m_ActiveIteratorIndices;
		std::vector<GroupViewInfo> m_GroupViews;
		
		static std::vector<std::function<void(uint8_t*)>> s_ComponentDestructors;
		static std::vector<size_t> s_ComponentSizes;

		static ComponentType s_NextComponentType;

		static const size_t NOT_FOUND = std::numeric_limits<size_t>::max();

		template<typename> friend class EntityView;
		template<typename> friend class ComponentView;
		template<typename> friend class PairView;
		template<typename, typename> friend class GroupView;
		friend class ComponentRegistry;
	};

}
