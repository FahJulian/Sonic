#pragma once
#include <algorithm>
#include "Sonic/Util/GenericContainer.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Renderer/Camera2D.h"
#include "Sonic/UI/SceneUIHandler.h"
#include "Views.h"
#include "Entity.h"
#include "Components.h"
#include "EntityGroup.h"
#include "ComponentType.h"
#include "ComponentRegistry.h"

namespace Sonic {

	template<typename Component1, typename Component2>
	class GroupView;

	class Scene : public EventDispatcher
	{
	protected:
		Scene();

	private:
		void Init();
		void Update(float deltaTime);
		void Rebuffer();
		void Render();

		void UpdateComponents(float deltaTime);
		void UpdateBehaviours(float deltaTime);

		void OnRenderer2DComponentAdded(const ComponentAddedEvent<Renderer2DComponent>& e);
		void OnTransform2DComponentAdded(const ComponentAddedEvent<Transform2DComponent>& e);

		void OnWindowResized(const WindowResizedEvent& e);

	protected:
		virtual void Load() = 0;
		virtual void OnInit() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void PollCollisionEvents() {}

	public:
		SceneUIHandler& GetUIHandler() { return m_UIHandler; }

		Entity AddEntity();
		Entity AddEntity(EntityGroup group);
		EntityGroup AddEntityGroup();
		void AddToGroup(EntityGroup group, Entity entity);
		void DeactivateEntity(Entity entity);
		void DeactivateEntities(EntityGroup group);
		void ReactivateEntity(Entity entity);
		void ReactivateEntities(EntityGroup group);
		void RemoveEntity(Entity entity);

		template<typename Component, typename... Args>
		void AddComponent(Entity entity, Args&&... args)
		{
			m_MainRegistry.AddComponent<Component>(entity, std::forward<Args>(args)...);
			DispatchEvent(ComponentAddedEvent<Component>(entity));
		}

		template<typename Component>
		bool HasComponent(Entity entity)
		{
			return m_MainRegistry.HasComponent<Component>(entity);
		}

		template<typename Component>
		Component* GetComponent(Entity entity)
		{
			return m_MainRegistry.GetComponent<Component>(entity);
		}

		template<typename Component>
		void RemoveComponent(Entity entity)
		{
			m_MainRegistry.RemoveComponent<Component>(entity);
			DispatchEvent(ComponentRemovedEvent<Component>(entity));
		}

		template<typename Component>
		EntityView ViewEntities()
		{
			return EntityView(&m_MainRegistry, getComponentType<Component>());
		}

		template<typename Component>
		ComponentView<Component> ViewComponents()
		{
			return ComponentView<Component>(&m_MainRegistry);
		}

		template<typename Component>
		PairView<Component> View()
		{
			return PairView<Component>(&m_MainRegistry);
		}

		template<typename Component1, typename Component2>
		GroupView<Component1, Component2>& Group()
		{
			static std::unordered_map<Scene*, GroupView<Component1, Component2>*> groups;

			auto it = groups.find(this);
			if (it == groups.end())
			{
				groups.emplace(this, new GroupView<Component1, Component2>(&m_MainRegistry));
				it = --(groups.end());
			}

			return *it->second;
		}

	private:
		Camera2D m_Camera;
		SceneUIHandler m_UIHandler;

		ComponentRegistry m_MainRegistry;
		ComponentRegistry m_InactiveRegistry;

		Entity m_NextEntity = 1;
		EntityGroup m_NextEntityGroup = 1;
		std::unordered_map<EntityGroup, std::vector<Entity>> m_EntityGroups;

		friend class App;
		template<typename, typename> friend class GroupView;
	};

}

