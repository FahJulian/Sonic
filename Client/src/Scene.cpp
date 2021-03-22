#include "sonic/core/scene/Scene.h"

#include "sonic/components/ui/UIComponent.hpp"

#include "TestScene.hpp"
#include "SceneData.hpp"

#ifdef SN_CLIENT_LINK_DYNAMIC
namespace sonic::client
{
	SN_CLIENT_API void initClientLog(Log* mainLog)
	{
		Log::init(mainLog);
	}
}
#endif

namespace sonic
{
	SN_CLIENT_API Scene::Scene(String&& name, SceneData* data)
		: mName(std::move(name)), mData(data)
	{
	}

	SN_CLIENT_API Optional<Scene*> Scene::createScene(const String& sceneName)
	{
		if (sceneName == "TestScene")
			return new client::TestScene();

		return NULLOPT;
	}

	SN_CLIENT_API void Scene::load(Map<String, Map<String, String>>& serializedScene)
	{
		Log::log(Log::INFO, "Loading scene ", mName, "...");

		Log::log(Log::INFO, "Done.");
	}

	SN_CLIENT_API void Scene::init()
	{
		Log::log(Log::INFO, "Initializing scene ", mName, "...");

		initComponentPools();
		initSystems();

		Log::log(Log::INFO, "Done.");
	}

	SN_CLIENT_API void Scene::update(float deltaTime)
	{
		updateComponents(deltaTime);
		updateSystems(deltaTime);
	}

	template<typename C>
	SN_CLIENT_API ComponentPool<C>* Scene::getComponentPool()
	{
		static_assert(false);
	}

	template<typename C>
	SN_CLIENT_API void Scene::onComponentAdded()
	{
		static_assert(false);
	}

	template<>
	SN_CLIENT_API ComponentPool<UIComponent>* Scene::getComponentPool<UIComponent>()
	{
		SN_ASSERT(mData->uiComponents != nullptr, "This scene does not use client::TestComponent.");

		return mData->uiComponents;
	}

	template<>
	SN_CLIENT_API void Scene::onComponentAdded<UIComponent>()
	{
	}

} // namespace sonic
