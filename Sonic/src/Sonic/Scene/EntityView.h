#pragma once
#include "Entity.h"
#include "ComponentType.h"
#include "ComponentPool.h"
#include "ComponentRegistry.h"

namespace Sonic {

	class EntityView
	{
		struct Iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = Entity;
			using reference = Entity&;
			using pointer = Entity*;

			ComponentPool* pool;
			size_t index;

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

			value_type operator*() { return pool->m_Entities[index]; }

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
		EntityView(ComponentRegistry* registry, ComponentType type)
			: m_Pool(registry->GetComponentPool(type))
		{
		}

		Iterator begin() { return Iterator(m_Pool, 0); }
		Iterator end() { return Iterator(m_Pool, m_Pool->m_Size); }

		void ForEach(std::function<void(Entity)> function)
		{
			for (auto it = begin(), endIt = end(); it < endIt; ++it)
				function(*it);
		}

		size_t Size()
		{
			return m_Pool->m_Size;
		}

		ComponentPool* m_Pool;
	};

}
