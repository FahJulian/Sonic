#pragma once
#include <string>
#include <vector>
#include "Sonic/Base.h"

namespace Sonic {

	class Scene;

	class SceneManager
	{
	public:	
		struct ManagedScene
		{
		public:
			String name;
			size_t sceneIndex;

		private:
			Scene* scene;
			bool isLoaded = false;

			ManagedScene(const String& name, size_t sceneIndex, Scene* scene)
				: name(name), sceneIndex(sceneIndex), scene(scene)
			{
			}

			friend bool operator==(const ManagedScene& scene1, const ManagedScene& scene2) { 
				return scene1.scene == scene2.scene; }

		public:
			Scene* operator->() { return scene; }
			Scene& operator*() { return *scene; }

			friend class SceneManager;
		};

		SceneManager() = delete;
		SceneManager(const SceneManager& other) = delete;

		static ManagedScene& getCurrentScene();
		static void registerScene(Scene* scene, const String& name);

		static void loadScene(const String& name);
		static void loadScene(size_t index);

		static void setScene(const String& name);
		static void setScene(size_t index);

	private:
		static void setScene(ManagedScene& scene);
		static void loadScene(ManagedScene& scene);

		static void init();

		static void destroy();

		static ManagedScene& s_CurrentScene;
		static std::vector<ManagedScene> s_Scenes;

		static ManagedScene INITIAL_SCENE;

		friend class App;
	};

}
