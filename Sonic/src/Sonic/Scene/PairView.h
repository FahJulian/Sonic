#pragma once
#include <vector>
#include <iterator>
#include "Sonic/Util/GenericContainer.h"
#include "ComponentPool.h"
#include "EntityID.h"

namespace Sonic {

	class Scene;

	template<typename Component>
	class PairView
	{
	public:
		struct Pair
		{
			EntityID entity;
			Component* component;
		};

		struct Iterator
		{
			using EntityIterator = std::vector<EntityID>::const_iterator;
			using ComponentIterator = typename std::vector<Component>::iterator;

			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Pair;
			using pointer = Pair*;
			using Reference = Pair&;

			Iterator(const EntityIterator& entityIterator, const ComponentIterator& componentIterator)
				: m_EntityIterator(entityIterator), m_ComponentIterator(componentIterator) {}

			value_type operator*() { return { *m_EntityIterator, &(*m_ComponentIterator) }; }

			Iterator& operator++() { m_EntityIterator++; m_ComponentIterator++; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
			
			friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_EntityIterator == b.m_EntityIterator && a.m_ComponentIterator == b.m_ComponentIterator; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return !(a == b); }

			EntityIterator m_EntityIterator;
			ComponentIterator m_ComponentIterator;
		};	

	public:
		PairView(Scene* scene)
			: m_Pool(GenericContainer::Get<ComponentPool<Component>, BaseComponentPool>(scene)) {}

		Iterator begin() { return Iterator(m_Pool->m_Entities.cbegin(), m_Pool->m_Components.begin()); }
		Iterator end() { return Iterator(m_Pool->m_Entities.cend(), m_Pool->m_Components.end()); }

	private:
		ComponentPool<Component>* m_Pool;

		friend class Scene;
	};

}
