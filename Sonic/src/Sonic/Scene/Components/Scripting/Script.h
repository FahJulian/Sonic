#pragma once
#include "Sonic/Scene/ECS/Entity.h"

namespace Sonic {

	class Scene;

	class Script
	{
	protected:
		virtual void OnInit() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnDestroy() {}

		~Script()
		{
			OnDestroy();
		}

	private:
		void Init(Scene* scene, Entity entity)
		{
			m_Scene = scene;
			m_Entity = entity;
			
			OnInit();
		}

	protected:
		Scene* m_Scene = nullptr;
		Entity m_Entity = 0;

		friend class Scene;
		friend struct ScriptComponent;
		friend class SceneSerializer;
		template<typename, typename, typename...> friend class Method;
	};

}
