#include "Sonic/Debug/Log/Log.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Scene.h"
#include "SceneManager.h"

using namespace Sonic;


static bool s_Initialized = false;

SceneManager::ManagedScene SceneManager::INITIAL_SCENE = SceneManager::ManagedScene("NULL", 0, nullptr);
SceneManager::ManagedScene & SceneManager::s_CurrentScene = INITIAL_SCENE;
std::vector<SceneManager::ManagedScene> SceneManager::s_Scenes;


SceneManager::ManagedScene& SceneManager::getCurrentScene()
{
	return s_CurrentScene;
}

void SceneManager::registerScene(Scene* scene, const String& name)
{
	s_Scenes.emplace_back(ManagedScene(name, s_Scenes.size(), scene));
}

void SceneManager::setScene(const String& name)
{
	for (ManagedScene& scene : s_Scenes)
	{
		if (scene.name == name)
		{
			setScene(scene);
			return;
		}
	}

	SONIC_LOG_ERROR("Cant set scene: No scene with the given name exists");
}

void SceneManager::setScene(size_t index)
{
	SONIC_LOG_DEBUG_ASSERT(index < s_Scenes.size(), "Cant set Scene: Bad index.");

	setScene(s_Scenes.at(index));
}

void SceneManager::setScene(ManagedScene& scene)
{
	if (!s_Initialized)
	{
		s_CurrentScene = scene;
		return;
	}

	if(!scene.isLoaded)
	{
		scene->Load();
		scene.isLoaded = true;
	}

	EventDispatcher::clear();
	s_CurrentScene->Destroy();
	s_CurrentScene = scene;
	s_CurrentScene->Init();
}

void SceneManager::loadScene(const String& name)
{
	for (ManagedScene& scene : s_Scenes)
	{
		if (scene.name == name)
		{
			loadScene(scene);
			return;
		}
	}

	SONIC_LOG_ERROR("Cant load scene: No scene with the given name exists");
}

void SceneManager::loadScene(size_t index)
{
	SONIC_LOG_DEBUG_ASSERT(index < s_Scenes.size(), "Cant set Scene: Bad index.");

	loadScene(s_Scenes.at(index));
}

void SceneManager::loadScene(ManagedScene& scene)
{
	if (!scene.isLoaded)
	{
		scene->Load();
		scene.isLoaded = true;
	}
}

void SceneManager::init()
{
	if (s_CurrentScene.scene == nullptr)
		s_CurrentScene = s_Scenes.at(0);

	s_CurrentScene->Load();
	s_CurrentScene->Init();

	s_Initialized = true;
}

void SceneManager::destroy()
{
	for (ManagedScene& scene : s_Scenes)
	{
		scene->Destroy();
		delete scene.scene;
	}
}
