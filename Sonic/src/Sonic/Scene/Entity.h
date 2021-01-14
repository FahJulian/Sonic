#pragma once
#include "EntityID.h"
#include "Scene.h"

namespace Sonic {

	class Entity
	{
		Entity(Scene* scene);
		Entity(Scene* scene, EntityID id);

	public:
		operator EntityID() { return m_ID; }

		bool operator==(const Entity& other) const { return m_ID == other.m_ID && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return m_ID != other.m_ID || m_Scene != other.m_Scene; }

		template<typename Component, typename... Args>
		void AddComponent(Args&&... args)
		{
			m_Scene->AddComponent<Component>(m_ID, std::forward<Args>(args)...);
		}

		template<typename Component>
		bool HasComponent()
		{
			m_Scene->HasComponent<Component>(m_ID);
		}

		template<typename Component>
		Component* GetComponent()
		{
			return m_Scene->GetComponent<Component>(m_ID);
		}

		template<typename Component>
		void RemoveComponent()
		{
			m_Scene->RemoveComponent<Component>(m_ID);
		}

		template<typename DerivedBehaviour, typename... Args>
		void AddBehaviour(Args&&... args)
		{
			m_Scene->AddBehaviour<DerivedBehaviour>(m_ID, std::forward<Args>(args)...);
		}

		template<typename DerivedBehaviour, typename... Args>
		void RemoveBehaviour()
		{
			m_Scene->RemoveBehaviour<DerivedBehaviour>(m_ID);
		}

		Scene* m_Scene;
		EntityID m_ID;

		friend class Scene;
		friend class Behaviour;
	};

}