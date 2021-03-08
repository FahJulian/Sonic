#pragma once
#include <vector>
#include <iterator>
#include <functional>
#include "Sonic/Event/Events/ECSEvents.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Entity.h"
#include "ComponentPool.h"


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

			Iterator(const Iterator& other) = delete;
			Iterator& operator=(const Iterator& other) = delete;

			value_type operator*() 
			{
				Entity entity = groupView->m_Entities[index]; 
				return {
					entity,
					pool1->GetComponent<Component1>(entity),
					pool2->GetComponent<Component2>(entity)
				}; 
			}

			Iterator& operator++() { index++; return *this; }

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.index == b.index; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.index != b.index; }
			friend bool operator <(const Iterator& a, const Iterator& b) { return a.index < b.index; }
			friend bool operator >(const Iterator& a, const Iterator& b) { return a.index > b.index; }
			friend bool operator <=(const Iterator& a, const Iterator& b) { return a.index <= b.index; }
			friend bool operator >=(const Iterator& a, const Iterator& b) { return a.index >= b.index; }

			~Iterator()
			{
				groupView->m_ActiveIteratorIndices.erase(std::remove(groupView->m_ActiveIteratorIndices.begin(),
					groupView->m_ActiveIteratorIndices.end(), &index));
			}
		};

	private:
		GroupView(ComponentPool* pool1, ComponentPool* pool2)
			: m_Pool1(pool1), m_Pool2(pool2)
		{
			for (size_t i = 0; i < m_Pool1->m_Active.size; i++)
			{
				Entity entity = m_Pool1->m_Active.entities[i];

				for (size_t j = 0; j < m_Pool2->m_Active.size; j++)
				{
					if (m_Pool2->m_Active.entities[j] == entity)
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

	public:
		Iterator begin() { return Iterator(m_Pool1, m_Pool2, this, 0); }
		Iterator end() { return Iterator(m_Pool1, m_Pool2, this, m_Entities.size()); }

		void ForEach(std::function<void(Entity, Component1*, Component2*)> function)
		{
			for (auto it = begin(), endIt = end(); it < endIt; ++it)
			{
				Group group = *it;
				function(group.entity, group.component1, group.component2);
			}
		}

		int Size()
		{
			return static_cast<int>(m_Entities.size());
		}

	private:
		ComponentPool* m_Pool1;
		ComponentPool* m_Pool2;
		std::vector<Entity> m_Entities;
		std::vector<size_t*> m_ActiveIteratorIndices;

		friend class ComponentRegistry;
	};

}
