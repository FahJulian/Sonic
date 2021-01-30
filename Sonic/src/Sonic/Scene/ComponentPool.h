#pragma once
#include <set>
#include <vector>
#include <unordered_map>
#include "Sonic/Log/Log.h"
#include "EntityID.h"

namespace Sonic {

	class BaseComponentPool
	{
	protected:
		virtual void OnUpdate() = 0;

		bool HasEntity(EntityID entity);
		void RemoveEntity(EntityID entity);
		int IndexOf(EntityID entity);

		std::vector<EntityID> m_Entities;
		std::set<int, std::greater<int>> m_ToRemove;

		friend class Scene;
	};


	template<typename Component>
	class ComponentPool : public BaseComponentPool
	{
		template<typename... Args>
		void AddComponent(EntityID entity, Args&&... args)
		{
#pragma warning(disable:4244)
			m_ToAdd.emplace(entity, Component(std::forward<Args>(args)...));
#pragma warning(default:4244)
		}

		Component* GetComponent(EntityID entity)
		{
#ifdef SONIC_DEBUG
			if (IndexOf(entity) == -1)
			{
				SONIC_LOG_DEBUG("Critical: ComponentPool::GetComponent: No component exists for this entity");
				return nullptr;
			}
#endif
			return &m_Components.at(IndexOf(entity));
		}

		void OnUpdate() override
		{
			for (int index : m_ToRemove)
			{
				m_Entities.erase(m_Entities.begin() + index);
				m_Components.erase(m_Components.begin() + index);
			}

			for (auto& [entity, component] : m_ToAdd)
			{
				m_Entities.push_back(entity);
				m_Components.push_back(component);
			}

			m_ToRemove.clear();
			m_ToAdd.clear();
		}

		std::vector<Component> m_Components;
		std::unordered_map<EntityID, Component> m_ToAdd;

		friend class Scene;
		template<typename> friend class EntityView;
		template<typename> friend class ComponentView;
		template<typename> friend class PairView;
		template<typename, typename> friend class GroupView;
	};

}

