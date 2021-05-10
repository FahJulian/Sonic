#include "sonic/core/scene/Scene.h"

#include "sonic/components/ui/UIComponent.hpp"

#include "TestScene.hpp"
#include "SceneData.hpp"

namespace sonic::client
{
	#ifdef SN_CLIENT_LINK_DYNAMIC
		SN_RCR_API void initClientLog(Log* mainLog)
		{
			Log::init(mainLog);
		}
	#endif

	SN_RCR_API Optional<Scene*> client::Scene::createScene(const String& sceneName)
	{
		if (sceneName == "TestScene")
			return new client::TestScene();

		return NULLOPT;
	}
}

namespace sonic
{
	

	SN_RCR_API Optional<Scene*> client::Scene::createScene(const String& sceneName)
	{
		if (sceneName == "TestScene")
			return new client::TestScene();

		return NULLOPT;
	}

	SN_RCR_API void client::Scene::load(Map<String, Map<String, String>>& serializedScene)
	{
		Log::log(Log::INFO, "Loading scene ", mName, "...");

		Log::log(Log::INFO, "Done.");
	}

	SN_RCR_API void client::Scene::init()
	{
		Log::log(Log::INFO, "Initializing scene ", mName, "...");

		initComponentPools();
		initSystems();

		Log::log(Log::INFO, "Done.");
	}

	SN_RCR_API void client::Scene::update(float deltaTime)
	{
		updateComponents(deltaTime);
		updateSystems(deltaTime);
	}

	template<typename C>
	SN_RCR_API ComponentPool<C>* client::Scene::getComponentPool()
	{
		static_assert(false);
	}

	template<typename C>
	SN_RCR_API void client::Scene::onComponentAdded()
	{
		static_assert(false);
	}

	template<>
	SN_RCR_API ComponentPool<UIComponent>* client::Scene::getComponentPool<UIComponent>()
	{
		SN_ASSERT(mData->uiComponents != nullptr, "This scene does not use client::TestComponent.");

		return mData->uiComponents;
	}

	template<>
	SN_RCR_API void client::Scene::onComponentAdded<UIComponent>()
	{
	}

} // namespace sonic
