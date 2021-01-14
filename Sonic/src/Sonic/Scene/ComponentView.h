#pragma once
#include <vector>
#include "EntityID.h"
#include "ComponentPool.h"

namespace Sonic {

	template<typename Component>
	class ComponentView
	{
	private:
		ComponentView(ComponentPool<Component>* pool)
			: m_Pool(pool) {}
	public:
		std::vector<EntityID>::const_iterator begin() { return m_Pool->m_Entities.cbegin(); }
		std::vector<EntityID>::const_iterator end() { return m_Pool->m_Entities.cend(); }

		Component* GetComponent(EntityID entity) { return m_Pool->GetComponent(entity); }
	private:
		ComponentPool<Component>* m_Pool;

		friend class Scene;
	};

}
