#pragma once
#include "EntityID.h"
#include <iostream>

namespace Sonic {

	class Scene;
	class Entity;

	class Behaviour
	{
	protected:
		virtual void OnInit() = 0;
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnDestroy() {}

		Scene* GetScene() const { return m_Scene; }
		EntityID GetEntityID() const { return m_EntityID; }
		Entity GetEntity() const;

		EntityID m_EntityID;
		Scene* m_Scene;

		friend class Scene;
		friend class BaseBehaviourPool;
	};

}
