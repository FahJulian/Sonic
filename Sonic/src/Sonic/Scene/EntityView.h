#pragma once
#include "Entity.h"
#include "ComponentType.h"
#include "ComponentPool.h"
#include "ComponentRegistry.h"

namespace Sonic {

	template<typename Component>
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

			value_type operator*() { return pool->m_Active.entities[index]; }

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
		EntityView(ComponentRegistry* registry)
			: m_Pool(registry->GetComponentPool<Component>())
		{
		}

		Iterator begin() { return Iterator(m_Pool, 0); }
		Iterator end() { return Iterator(m_Pool, m_Pool->m_Active.size); }

		void ForEach(std::function<void(Entity)> function)
		{
			for (auto it = begin(), endIt = end(); it < endIt; ++it)
				function(*it);
		}

		int Size()
		{
			return static_cast<int>(m_Pool->m_Active.size);
		}

		ComponentPool* m_Pool;
	};

}
