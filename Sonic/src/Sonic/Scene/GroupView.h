#pragma once
#include <iterator>
#include <unordered_set>
#include "Sonic/Util/GenericContainer.h"
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

		struct Iterator
		{
			using EntityIterator = std::unordered_set<EntityID>::const_iterator;
			using Component1Iterator = typename std::vector<Component1>::iterator;
			using Component2Iterator = typename std::vector<Component1>::iterator;

			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Group;
			using pointer = Group*;
			using Reference = Group&;

			Iterator(const EntityIterator& entityIterator, ComponentPool<Component1>* pool1, ComponentPool<Component2>* pool2)
				: entityIterator(entityIterator), pool1(pool1), pool2(pool2) {}

			value_type operator*() { return { *entityIterator, pool1->GetComponent(*entityIterator), pool2->GetComponent(*entityIterator) }; }

			Iterator& operator++() { entityIterator++; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.entityIterator == b.entityIterator; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.entityIterator != b.entityIterator; }

			EntityIterator entityIterator;
			ComponentPool<Component1>* pool1;
			ComponentPool<Component2>* pool2;
		};

	public:
		GroupView(Scene* scene)
			: m_Pool1(GenericContainer::Get<ComponentPool<Component1>, BaseComponentPool>(scene)), m_Pool2(GenericContainer::Get<ComponentPool<Component2>, BaseComponentPool>(scene))
		{
			for (EntityID e1 : m_Pool1->m_Entities)
				if (m_Pool2->HasEntity(e1) || m_Pool2->m_ToAdd.find(e1) != m_Pool2->m_ToAdd.end())
					m_Entities.insert(e1);

			for (auto [e1, _] : m_Pool1->m_ToAdd)
				if (m_Pool2->HasEntity(e1) || m_Pool2->m_ToAdd.find(e1) != m_Pool2->m_ToAdd.end())
					m_Entities.insert(e1);

			scene->AddListener<Scene::ComponentAddedEvent<Component1>>([this, scene](const Scene::ComponentAddedEvent<Component1>& e) {
				if (scene->HasComponent<Component2>(e.entity))
					m_Entities.insert(e.entity);
			});

			scene->AddListener<Scene::ComponentAddedEvent<Component2>>([this, scene](const Scene::ComponentAddedEvent<Component2>& e) {
				if (scene->HasComponent<Component1>(e.entity))
					m_Entities.insert(e.entity);
			});

			scene->AddListener<Scene::ComponentRemovedEvent<Component1>>([this, scene](const Scene::ComponentRemovedEvent<Component1>& e) {
				std::unordered_set<EntityID>::iterator it = m_Entities.find(e.entity);
				if (it != m_Entities.end())
					m_Entities.erase(it);
			});

			scene->AddListener<Scene::ComponentRemovedEvent<Component2>>([this, scene](const Scene::ComponentRemovedEvent<Component2>& e) {
				std::unordered_set<EntityID>::iterator it = m_Entities.find(e.entity);
				if (it != m_Entities.end())
					m_Entities.erase(it);
			});
		}

		Iterator begin() { return Iterator(m_Entities.cbegin(), m_Pool1, m_Pool2); }
		Iterator end() { return Iterator(m_Entities.cend(), m_Pool1, m_Pool2); }

	private:
		std::unordered_set<EntityID> m_Entities;
		ComponentPool<Component1>* m_Pool1;
		ComponentPool<Component2>* m_Pool2;

		friend class Scene;
	};

}
