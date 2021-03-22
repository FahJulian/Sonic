#pragma once

#include "sonic/core/scene/Entity.h"

namespace sonic
{
	template<typename C1, typename C2>
	class GroupView
	{
	public:
		struct Group
		{
			Entity entity;
			//C1& component1;
			//C2& component2;
		};

		class Iterator
		{
		public:
			Group& operator*()
			{
				static Group group; // placeholder
				return group;
			}

			Iterator& operator++()
			{
				return *this;
			}

			bool operator!=(const Iterator& other)
			{
				return false;
			}
		};

		Iterator begin()
		{
			return { };
		}

		Iterator end()
		{
			return { };
		}
	};

} // namespace sonic
