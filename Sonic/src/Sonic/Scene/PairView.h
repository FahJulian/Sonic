#pragma once
#include "ComponentType.h"
#include "ComponentPool.h"
#include "ComponentRegistry.h"

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

			Iterator(const Iterator& other) = delete;
			void operator=(const Iterator& other) = delete;

			value_type operator*() { return { pool->m_Entities[index], reinterpret_cast<Component*>(pool->m_Data) + index }; }

			Iterator& operator++() { index++; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.index == b.index; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { 
				return a.index != b.index; 
			}

			~Iterator()
			{
				pool->m_ActiveIteratorIndices.erase(std::remove(pool->m_ActiveIteratorIndices.begin(), pool->m_ActiveIteratorIndices.end(), &index));
			}
		};

	public:
		PairView(ComponentRegistry* registry)
			: m_Pool(registry->GetComponentPool(getComponentType<Component>()))
		{
		}

		Iterator begin() { return Iterator(m_Pool, 0); }
		Iterator end() { return Iterator(m_Pool, m_Pool->m_Size); }

		void ForEach(std::function<void(Entity entity, Component* component)> function)
		{
			for (auto [entity, component] : *this)
				function(entity, component);
		}

		size_t Size()
		{
			return m_Pool->m_Size;
		}

		ComponentPool* m_Pool;
	};

}
