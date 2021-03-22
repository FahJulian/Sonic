#pragma once

#include "sonic/core/scene/Entity.h"

namespace sonic
{
	template<typename C>
	class ComponentPool
	{
	public:
		template<typename... Args>
		void addComponent(Entity entity, Args&&... args)
		{
			C c = C(std::forward<Args>(args)...);
		}

		C* begin()
		{
			return nullptr;
		}

		C* end()
		{
			return nullptr;
		}
	};
}
