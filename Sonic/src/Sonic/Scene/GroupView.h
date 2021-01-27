#pragma once
#include "Sonic/Debug/Profiler/Profiler.h"
#include <iterator>
#include <unordered_set>
#include "EntityID.h"
#include "ComponentPool.h"

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

	private:
		GroupView(ComponentPool<Component1>* pool1, ComponentPool<Component2>* pool2)
			: pool1(pool1), pool2(pool2) 
		{
			for (EntityID e1 : pool1->m_Entities)
			{
				for (EntityID e2 : pool2->m_Entities)
				{
					if (e2 == e1)
					{
						entities.insert(e1);
						break;
					}
				}
			}
		}

	public:
		Iterator begin() { return Iterator(entities.cbegin(), pool1, pool2); }
		Iterator end() { return Iterator(entities.cend(), pool1, pool2); }

	private:
		std::unordered_set<EntityID> entities;
		ComponentPool<Component1>* pool1;
		ComponentPool<Component2>* pool2;

		friend class Scene;
	};

}
