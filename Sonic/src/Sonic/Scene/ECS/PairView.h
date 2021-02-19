#pragma once
#include <iterator>
#include <functional>
#include "Entity.h"
#include "ComponentPool.h"

namespace Sonic {

	template<typename Component>
	class PairView
	{
		struct Pair
		{
			Entity entity;
			Component* component;
		};

		struct Iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = Pair;
			using reference = Pair&;
			using pointer = Pair*;

			size_t index;
			ComponentPool* pool;

			Iterator(ComponentPool* pool, size_t index)
				: pool(pool), index(index)
			{
				pool->m_ActiveIteratorIndices.push_back(&this->index);
			}

			Iterator(const Iterator& other)
				: pool(other.pool), index(other.index)
			{
				pool->m_ActiveIteratorIndices.push_back(&this->index);
			}

			value_type operator*() { return { pool->m_Active.entities[index], reinterpret_cast<Component*>(pool->m_Active.data) + index }; }

			Iterator& operator++() { index++; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.index == b.index; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.index != b.index; }
			friend bool operator <(const Iterator& a, const Iterator& b) { return a.index < b.index; }
			friend bool operator >(const Iterator& a, const Iterator& b) { return a.index > b.index; }
			friend bool operator <=(const Iterator& a, const Iterator& b) { return a.index <= b.index; }
			friend bool operator >=(const Iterator& a, const Iterator& b) { return a.index >= b.index; }

			~Iterator()
			{
				pool->m_ActiveIteratorIndices.erase(std::remove(pool->m_ActiveIteratorIndices.begin(), 
					pool->m_ActiveIteratorIndices.end(), &index));
			}
		};

	public:
		PairView(ComponentPool* pool)
			: m_Pool(pool)
		{
		}

		Iterator begin() { return Iterator(m_Pool, 0); }
		Iterator end() { return Iterator(m_Pool, m_Pool->m_Active.size); }

		void ForEach(std::function<void(Entity, Component*)> function)
		{
			for (auto it = begin(), endIt = end(); it < endIt; ++it)
			{
				Pair pair = *it;
				function(pair.entity, pair.component);
			}
		}

		int Size()
		{
			return static_cast<int>(m_Pool->m_Active.size);
		}

		ComponentPool* m_Pool;
	};

}
