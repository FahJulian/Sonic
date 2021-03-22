#include "sonic/core/scene/Scene.h"

#include "SceneData.hpp"

namespace sonic::client
{
	class TestScene : public Scene
	{
	public:
		TestScene()
			: Scene("TestScene", &mData), mUiRenderer()
		{
		}

		void initComponentPools() override
		{
			mData.uiComponents = &mUIComponents;
		}

		void initSystems() override
		{
			mData.uiRenderer = &mUiRenderer;
		}

		void updateComponents(float deltaTime) override
		{
			for (auto& behaviour : m_client_testBehaviours)
				behaviour.update(deltaTime);
		}

		void updateSystems(float deltaTime) override
		{
			mUiRenderer.update(deltaTime, group<UIComponent, UIRendererComponent>());
		}

		SceneData mData;
		UIRenderer mUiRenderer;
		ComponentPool<UIComponent> mUIComponents;
		ComponentPool<client::TestBehaviour> m_client_testBehaviours;
	};

} // namespace sonic::client
