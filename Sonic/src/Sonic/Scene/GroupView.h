#pragma once
#include <vector>
#include <iterator>
#include <functional>
#include "Sonic/Event/Events.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Entity.h"
#include "ComponentType.h"
#include "ComponentPool.h"
#include "ComponentRegistry.h"

namespace Sonic {

	template<typename Component1, typename Component2>
	class GroupView
	{
		struct Group
		{
			Entity entity;
			Component1* component1;
			Component2* component2;
		};

		struct Iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = Group;
			using reference = Group&;
			using pointer = Group*;

			ComponentPool* pool1;
			ComponentPool* pool2;
			GroupView* groupView;
			size_t index;

			Iterator(ComponentPool* pool1, ComponentPool* pool2, GroupView<Component1, Component2>* groupView, size_t index)
				: pool1(pool1), pool2(pool2), groupView(groupView), index(index)
			{
				groupView->m_ActiveIteratorIndices.push_back(&this->index);
			}

			Iterator(const Iterator& other)
				: pool1(other.pool1), pool2(other.pool2), groupView(other.groupView), index(other.index)
			{
				groupView->m_ActiveIteratorIndices.push_back(&this->index);
			}

			value_type operator*() 
			{
				Entity entity = groupView->m_Entities[index]; 
				return {
					entity,
					pool1->GetComponent<Component1>(entity),
					pool2->GetComponent<Component2>(entity)
				};
			}

			Iterator operator++() { index++; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.index == b.index; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.index != b.index; }

			~Iterator()
			{
				groupView->m_ActiveIteratorIndices.erase(std::remove(groupView->m_ActiveIteratorIndices.begin(),
					groupView->m_ActiveIteratorIndices.end(), &index));
			}
		};

	public:
		GroupView(ComponentRegistry* registry)
			: m_Pool1(registry->GetComponentPool(getComponentType<Component1>())), m_Pool2(registry->GetComponentPool(getComponentType<Component2>()))
		{
			for (size_t i = 0; i < m_Pool1->m_Size; i++)
			{
				Entity entity = m_Pool1->m_Entities[i];

				for (size_t j = 0; j < m_Pool2->m_Size; j++)
				{
					if (m_Pool2->m_Entities[j] == entity)
					{
						m_Entities.emplace_back(entity);
						break;
					}
				}
			}

			m_Pool1->m_GroupViews.emplace_back(m_Pool2, &m_Entities, &m_ActiveIteratorIndices);
			m_Pool2->m_GroupViews.emplace_back(m_Pool1, &m_Entities, &m_ActiveIteratorIndices);
		}

		GroupView(const GroupView& other) = delete;
		GroupView& operator=(const GroupView& other) = delete;

		Iterator begin() { return Iterator(m_Pool1, m_Pool2, this, 0); }
		Iterator end() { return Iterator(m_Pool1, m_Pool2, this, m_Entities.size()); }

		void ForEach(std::function<void(Entity, Component1*, Component2*)> function)
		{
			for (auto [entity, component1, component2] : *this)
				function(entity, component1, component2);
		}

		size_t Size()
		{
			return m_Entities.size();
		}

		ComponentPool* m_Pool1;
		ComponentPool* m_Pool2;
		std::vector<Entity> m_Entities;
		std::vector<size_t*> m_ActiveIteratorIndices;
	};

}
