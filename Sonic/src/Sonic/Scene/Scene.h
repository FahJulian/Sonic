#pragma once
#include <algorithm>
#include "Sonic/Event/Events.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Renderer/2D/Camera2D.h"
#include "Sonic/Scene/UI/SceneUIHandler.h"
#include "Sonic/Scene/ECS/ComponentRegistry.h"
#include "Sonic/Scene/Components/2DComponents.h"
#include "Sonic/Scene/Components/Scripting/ScriptComponent.h"

namespace Sonic {

	template<typename Component1, typename Component2>
	class GroupView;

	class Scene : public ComponentRegistry
	{
	protected:
		Scene()
			: ComponentRegistry::ComponentRegistry(),
			m_Camera(Camera2D(0, Window::getWidth(), 0, Window::getHeight())), m_UIHandler(this)
		{
		}

	private:
		void Load();
		void Init();

		void Save();

		void Update(float deltaTime);

		void Destroy();

		void RebufferRenderers();
		void Render();

		void UpdateComponents(float deltaTime);

		void OnRenderer2DComponentAdded(const ComponentAddedEvent<Renderer2DComponent>& e);
		void OnTransform2DComponentAdded(const ComponentAddedEvent<Transform2DComponent>& e);

		void OnScriptComponentAdded(const ComponentAddedEvent<ScriptComponent>& e);
		void OnScriptComponentDeactivated(const ComponentDeactivatedEvent<ScriptComponent>& e);
		void OnScriptComponentReactivated(const ComponentReactivatedEvent<ScriptComponent>& e);

		void OnEntityReactivated(const EntityReactivatedEvent& e);
		void OnEntityDeactivated(const EntityDeactivatedEvent& e);

		void OnWindowResized(const WindowResizedEvent& e);

	protected:
		virtual void OnLoad() = 0;
		virtual void OnInit() {}
		virtual void OnSave() {}

		virtual void OnUpdate(float deltaTime) {}

		virtual void PollCollisionEvents() {}

		virtual ~Scene() {}

	public:
		SceneUIHandler& GetUIHandler() { return m_UIHandler; }

	private:
		Camera2D m_Camera;
		SceneUIHandler m_UIHandler;

		friend class App;
		friend class SceneManager;
		friend class SceneSerializer;
		template<typename, typename> friend class GroupView;
	};

}

