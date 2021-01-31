#pragma once
#include <vector>
#include "Sonic/Util/GenericContainer.h"
#include "ComponentPool.h"
#include "EntityID.h"

namespace Sonic {

	class Scene;

	template<typename Component>
	class EntityView
	{
	public:
		EntityView(Scene* scene)
			: m_Pool(GenericContainer::GetOrAddWithBase<ComponentPool<Component>, BaseComponentPool, Scene, EventDispatcher*>(scene, (EventDispatcher*)scene))
		{
		}

		std::vector<EntityID>::const_iterator begin() { return m_Pool->m_Entities.cbegin(); }
		std::vector<EntityID>::const_iterator end() { return m_Pool->m_Entities.cend(); }

		Component* GetComponent(EntityID entity) { return m_Pool->GetComponent(entity); }

	private:
		ComponentPool<Component>* m_Pool;
	};

}
