#pragma once
#include "Entity.h"
#include "Scene.h"

namespace Sonic {

	class Script
	{
	protected:
		virtual void OnInit() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnDestroy() {}

	private:
		void Init(Scene* scene, Entity entity)
		{
			m_Scene = scene;
			m_Entity = entity;
			
			OnInit();
		}

	protected:
		Scene* m_Scene;
		Entity m_Entity;

		friend class Scene;
	};

}
