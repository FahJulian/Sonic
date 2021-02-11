#pragma once
#include "ComponentType.h"
#include "ComponentPool.h"
#include "ComponentRegistry.h"

namespace Sonic {

	template<typename Component>
	class ComponentView
	{
		struct Iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = Component;
			using reference = Component&;
			using pointer = Component*;

			size_t index;
			AnonymousComponentPool* pool;

			Iterator(AnonymousComponentPool* pool, size_t index)
				: pool(pool), index(index)
			{
				pool->m_ActiveIteratorIndices.push_back(&this->index);
			}

			Iterator(const Iterator& other)
				: pool(other.pool), index(other.index)
			{
				pool->m_ActiveIteratorIndices.push_back(&this->index);
			}

			pointer operator*() { return reinterpret_cast<Component*>(pool->m_Data) + index; }

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
		ComponentView(ComponentRegistry* registry)
			: m_Pool(registry->GetComponentPool<Component>())
		{
		}

		Iterator begin() { return Iterator(m_Pool, 0); }
		Iterator end() { return Iterator(m_Pool, m_Pool->m_Size); }

		void ForEach(std::function<void(Component*)> function)
		{
			for (auto it = begin(), endIt = end(); it < endIt; ++it)
				function(*it);
		}

		int Size()
		{
			return static_cast<int>(m_Pool->m_Size);
		}

		AnonymousComponentPool* m_Pool;
	};

}
