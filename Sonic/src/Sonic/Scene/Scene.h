#pragma once
#include <algorithm>
#include "Sonic/Window/Window.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Renderer/2D/Camera2D.h"
#include "Sonic/Scene/UI/SceneUIHandler.h"
#include "Sonic/Scene/ECS/Views.h"
#include "Sonic/Scene/ECS/Entity.h"
#include "Sonic/Scene/ECS/ComponentType.h"
#include "Sonic/Scene/ECS/ComponentRegistry.h"
#include "Sonic/Scene/ECS/EntityGroup.h"
#include "Sonic/Scene/Components/2DComponents.h"

namespace Sonic {

	template<typename Component1, typename Component2>
	class GroupView;

	class Scene : public EventDispatcher
	{
	protected:
		Scene()
			: m_Camera(Camera2D(0, Window::getWidth(), 0, Window::getHeight())), m_UIHandler(this)
		{
		}

	private:
		void Load();
		void Init();

		void Update(float deltaTime);

		void Destroy();

		void RebufferRenderers();
		void Render();

		void UpdateComponents(float deltaTime);

		void OnRenderer2DComponentAdded(const ComponentAddedEvent<Renderer2DComponent>& e);
		void OnTransform2DComponentAdded(const ComponentAddedEvent<Transform2DComponent>& e);

		void OnWindowResized(const WindowResizedEvent& e);

	protected:
		virtual void OnLoad() = 0;
		virtual void OnInit() {}

		virtual void OnUpdate(float deltaTime) {}

		virtual void PollCollisionEvents() {}

		virtual ~Scene() {}

	public:
		SceneUIHandler& GetUIHandler() { return m_UIHandler; }

		Entity AddEntity();
		Entity AddEntity(EntityGroup group);
		EntityGroup AddEntityGroup();
		std::vector<Entity>* GetGroup(EntityGroup group);
		void AddToGroup(EntityGroup group, Entity entity);

		void DeactivateEntity(Entity entity);
		void DeactivateEntities(EntityGroup group);
		void ReactivateEntity(Entity entity);
		void ReactivateEntities(EntityGroup group);
		void RemoveEntity(Entity entity);

		template<typename Component, typename... Args>
		void AddComponent(Entity entity, Args&&... args)
		{
			m_Registry.AddComponent<Component>(entity, std::forward<Args>(args)...);
			DispatchEvent(ComponentAddedEvent<Component>(entity));
		}

		template<typename Component>
		bool HasComponent(Entity entity)
		{
			return m_Registry.HasComponent<Component>(entity);
		}

		template<typename Component>
		Component* GetComponent(Entity entity)
		{
			return m_Registry.GetComponent<Component>(entity);
		}

		template<typename Component>
		void RemoveComponent(Entity entity)
		{
			m_Registry.RemoveComponent<Component>(entity);
			DispatchEvent(ComponentRemovedEvent<Component>(entity));
		}

		template<typename Component>
		EntityView<Component> ViewEntities()
		{
			return EntityView<Component>(&m_Registry);
		}

		template<typename Component>
		ComponentView<Component> ViewComponents()
		{
			return ComponentView<Component>(&m_Registry);
		}

		template<typename Component>
		PairView<Component> View()
		{
			return PairView<Component>(&m_Registry);
		}

		template<typename Component1, typename Component2>
		GroupView<Component1, Component2>& Group()
		{
			static std::unordered_map<Scene*, GroupView<Component1, Component2>*> groups;

			auto it = groups.find(this);
			if (it == groups.end())
			{
				groups.emplace(this, new GroupView<Component1, Component2>(&m_Registry));
				it = --(groups.end());
			}

			return *it->second;
		}

	private:
		Camera2D m_Camera;
		SceneUIHandler m_UIHandler;

		ComponentRegistry m_Registry;

		Entity m_NextEntity = 1;
		std::unordered_map<EntityGroup, std::vector<Entity>> m_EntityGroups;

		friend class App;
		friend class SceneManager;
		template<typename, typename> friend class GroupView;
	};

}

