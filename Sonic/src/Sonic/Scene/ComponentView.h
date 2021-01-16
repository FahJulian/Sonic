#pragma once
#include <vector>
#include <iterator>
#include "ComponentPool.h"

namespace Sonic {

	template<typename Component>
	class ComponentView
	{
		struct Iterator
		{
			using ComponentIterator = typename std::vector<Component>::iterator;

			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Component*;
			using pointer = Component**;
			using reference = Component*&;

			Iterator(ComponentIterator componentIterator)
				: m_ComponentIterator(componentIterator) {}

			value_type operator*() { return &(*m_ComponentIterator); }

			Iterator& operator++() { m_ComponentIterator++; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ComponentIterator == b.m_ComponentIterator; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return !(a == b); }

			ComponentIterator m_ComponentIterator;
		};

	private:
		ComponentView(ComponentPool<Component>* pool)
			: m_Pool(pool) {}

	public:
		Iterator begin() { return { m_Pool->m_Components.begin() }; }
		Iterator end() { return { m_Pool->m_Components.end() };
	}

	private:
		ComponentPool<Component>* m_Pool;

		friend class Scene;
	};

}
