#pragma once
#include <set>
#include <vector>
#include <unordered_map>
#include "EntityID.h"
#include "Behaviour.h"

namespace Sonic {

	class Scene;

	class BaseBehaviourPool
	{
	protected:
		BaseBehaviourPool(Scene* scene)
			: m_Scene(scene) {}

		void UpdateBehaviours(float deltaTime);
		void UpdatePool();

		bool HasEntity(EntityID entity);
		void RemoveEntity(EntityID entity);
		int IndexOf(EntityID entity);

		Scene* m_Scene;

		std::vector<EntityID> m_Entites;
		std::set<int, std::greater<int>> m_ToRemove;

		std::vector<Behaviour*> m_Behaviours;
		std::unordered_map<EntityID, Behaviour*> m_ToAdd;

		friend class Scene;
	};


	template<typename DerivedBehaviour>
	class BehaviourPool : private BaseBehaviourPool
	{
	private:
		BehaviourPool(Scene* scene)
			: BaseBehaviourPool(scene) {}

		template<typename... Args>
		void AddBehaviour(EntityID entity, Args&&... args)
		{
#pragma warning(disable:4244)
			Behaviour* behaviour = static_cast<Behaviour*>(new DerivedBehaviour(std::forward<Args>(args)...));
#pragma warning(default:4244)
			m_ToAdd.emplace(entity, behaviour);
		}

		friend class Scene;
	};

}
