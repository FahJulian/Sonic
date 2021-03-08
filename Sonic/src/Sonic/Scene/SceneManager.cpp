#include "Sonic/Debug/Log/Log.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Scene.h"
#include "SceneManager.h"

using namespace Sonic;


static bool s_Initialized = false;

bool SceneManager::s_ChangeScheduled = false;
SceneManager::ManagedScene SceneManager::s_CurrentScene = SceneManager::ManagedScene("NULL", 0, nullptr);
SceneManager::ManagedScene SceneManager::s_ChangedScene = SceneManager::ManagedScene("NULL", 0, nullptr);
std::vector<SceneManager::ManagedScene> SceneManager::s_Scenes;


const SceneManager::ManagedScene& SceneManager::getCurrentScene()
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
		if (*scene.name == name)
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
	else
	{
		s_ChangedScene = scene;
		s_ChangeScheduled = true;
	}
}

bool SceneManager::isSceneChangeScheduled()
{
	return s_ChangeScheduled;
}

void SceneManager::executeSceneChange()
{
	loadScene(s_ChangedScene);
	EventDispatcher::clear();
	s_CurrentScene->Destroy();
	*s_CurrentScene.isLoaded = false;
	s_ChangedScene->Init();
	s_CurrentScene = s_ChangedScene;
	s_ChangeScheduled = false;
}

void SceneManager::loadScene(const String& name)
{
	for (ManagedScene& scene : s_Scenes)
	{
		if (*scene.name == name)
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
	if (!*scene.isLoaded)
	{
		scene->Load();
		*scene.isLoaded = true;
	}
}

void SceneManager::init()
{
	if (s_CurrentScene.scene == nullptr)
		s_CurrentScene = s_Scenes.at(0);

	loadScene(s_CurrentScene);
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
