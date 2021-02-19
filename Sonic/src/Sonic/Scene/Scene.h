#pragma once
#include <algorithm>
#include "Sonic/Event/Events.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Renderer/2D/Camera2D.h"
#include "Sonic/Scene/UI/SceneUIHandler.h"
#include "Sonic/Scene/ECS/ComponentRegistry.h"
#include "Sonic/Scene/Components/2DComponents.h"

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

	private:
		Camera2D m_Camera;
		SceneUIHandler m_UIHandler;

		friend class App;
		friend class SceneManager;
		template<typename, typename> friend class GroupView;
	};

}

