#pragma once
#include <functional>
#include "Scene.h"

namespace Sonic {

	class Behaviour
	{
	protected:
		virtual void OnInit() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnDestroy() {}

		template<typename Component>
		bool HasComponent()
		{
			return scene->HasComponent<Component>(entity);
		}

		template<typename Component>
		Component* GetComponent()
		{
			return scene->GetComponent<Component>(entity);
		}

		template<typename Component>
		void AddComponent(const Component& component)
		{
			scene->AddComponent(entity, component);
		}

		template<typename Component, typename... Args>
		void AddComponent(Args&&... args)
		{
			scene->AddComponent<Component>(entity, std::forward<Args>(args)...);
		}

		void RemoveEntity()
		{
			scene->RemoveEntity(entity);
		}

		template<typename F, typename Event>
		std::shared_ptr<int> AddRemovableListener(F* const obj, void(F::* method)(const Event&))
		{
			return scene->AddRemovableListener(obj, method);
		}

		template<typename Event>
		std::shared_ptr<int> AddRemovableListener(std::function<void(const Event&)> listener)
		{
			return scene->AddRemovableListener(listener);
		}

		template<typename Event>
		void RemoveEventListener(const std::shared_ptr<int>& index)
		{
			scene->RemoveEventListener<Event>(index);
		}

		Scene* GetScene() { return scene; }

		const Entity GetEntity() { return entity; }

	private:
		Entity entity;
		Scene* scene;

		friend class Scene;
	};

}
