#pragma once
#include <iterator>
#include <unordered_map>
#include <functional>
#include "Sonic/Util/GenericContainer.h"
#include "Sonic/Event/Events.h"
#include "ComponentPool.h"
#include "EntityID.h"
#include "Scene.h"

namespace Sonic {

	template<typename Component1, typename Component2>
	class GroupView
	{
	public: 
		struct Group
		{
			EntityID entity;
			Component1* component1;
			Component2* component2;
		};

		struct IndexPair
		{
			int index1;
			int index2;
		};

		struct Iterator
		{
			using MapIterator = typename std::unordered_map<EntityID, IndexPair>::const_iterator;

			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Group;
			using pointer = Group*;
			using Reference = Group&;

			Iterator(const MapIterator& mapIterator, ComponentPool<Component1>* pool1, ComponentPool<Component2>* pool2)
				: mapIterator(mapIterator), pool1(pool1), pool2(pool2) 
			{
			}

			value_type operator*() { return { mapIterator->first, &pool1->m_Components.at(mapIterator->second.index1), &pool2->m_Components.at(mapIterator->second.index2) }; }

			Iterator& operator++() { mapIterator++; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.mapIterator == b.mapIterator; }
			friend bool operator!=(const Iterator& a, const Iterator& b) 
			{ 
				return a.mapIterator != b.mapIterator; 
			}

			MapIterator mapIterator;
			ComponentPool<Component1>* pool1;
			ComponentPool<Component2>* pool2;
		};

	public:
		GroupView(Scene* scene)
			: m_Pool1(GenericContainer::GetOrAddWithBase<ComponentPool<Component1>, BaseComponentPool, Scene, EventDispatcher*>(scene, (EventDispatcher*)scene)),
			m_Pool2(GenericContainer::GetOrAddWithBase<ComponentPool<Component2>, BaseComponentPool, Scene, EventDispatcher*>(scene, (EventDispatcher*)scene))
		{
			for (int idx1 = 0; idx1 < m_Pool1->m_Entities.size(); idx1++)
			{
				EntityID e1 = m_Pool1->m_Entities.at(idx1);

				int idx2 = m_Pool2->IndexOf(e1); 
				if (idx2 != -1)
					m_Elements.emplace(e1, IndexPair{ idx1, idx2 });
			}

			scene->AddListener<ComponentAddedEvent<Component1>>([this](const ComponentAddedEvent<Component1>& e) {
				if (m_Pool2->HasEntity(e.entity))
					m_Elements.emplace(e.entity, IndexPair{ m_Pool1->IndexOf(e.entity), m_Pool2->IndexOf(e.entity) });
			});

			scene->AddListener<ComponentAddedEvent<Component2>>([this](const ComponentAddedEvent<Component2>& e) {
				if (m_Pool1->HasEntity(e.entity))
					m_Elements.emplace(e.entity, IndexPair{ m_Pool1->IndexOf(e.entity), m_Pool2->IndexOf(e.entity) });
			});

			scene->AddListener<ComponentRemovedEvent<Component1>>([this](const ComponentRemovedEvent<Component1>& e) {
				typename std::unordered_map<EntityID, IndexPair>::iterator it = m_Elements.find(e.entity);
				if (it != m_Elements.end())
					m_Elements.erase(it);
			});

			scene->AddListener<ComponentRemovedEvent<Component2>>([this](const ComponentRemovedEvent<Component2>& e) {
				typename std::unordered_map<EntityID, IndexPair>::iterator it = m_Elements.find(e.entity);
				if (it != m_Elements.end())
					m_Elements.erase(it);
			});
		}

		Iterator begin() { return Iterator(m_Elements.cbegin(), m_Pool1, m_Pool2); }
		Iterator end() { return Iterator(m_Elements.cend(), m_Pool1, m_Pool2); }

		void ForEach(std::function<void(EntityID e, Component1* c1, Component2* c2)> function)
		{
			Iterator end = end();
			for (Iterator iter = begin(); iter != end; iter++)
			{
				Group group = *iter;
				function(group->entity, group->component1, group->component2);
			}
		}

		int Size()
		{
			return static_cast<int>(m_Elements.size());
		}

	private:
		std::unordered_map<EntityID, IndexPair> m_Elements;
		ComponentPool<Component1>* m_Pool1;
		ComponentPool<Component2>* m_Pool2;

		friend class Scene;
	};

}
