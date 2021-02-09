#pragma once
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

			size_t index;
			ComponentPool* pool;

			Iterator(ComponentPool* pool, size_t index)
				: pool(pool), index(index)
			{
				pool->m_ActiveIteratorIndices.push_back(&this->index);
			}

			value_type operator*() { return pool->m_Entities[index]; }

			Iterator& operator++() { index++; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.index == b.index; }
			friend bool operator!=(const Iterator& a, const Iterator& b)
			{
				return a.index != b.index;
			}

			~Iterator()
			{
				pool->m_ActiveIteratorIndices.erase(std::remove(pool->m_ActiveIteratorIndices.begin(), pool->m_ActiveIteratorIndices.end(), &index));
			}
		};

	public:
		EntityView(ComponentRegistry* registry, ComponentType type)
			: m_Pool(registry->GetComponentPool(type))
		{
		}

		Iterator begin() { return Iterator(m_Pool, 0); }
		Iterator end() { return Iterator(m_Pool, m_Pool->m_Size); }

		void ForEach(std::function<void(Entity entity)> function)
		{
			for (auto entity : *this)
				function(entity);
		}

		size_t Size()
		{
			return m_Pool->m_Size;
		}

		ComponentPool* m_Pool;
	};

}
