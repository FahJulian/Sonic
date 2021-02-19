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
			Ref<String> name;
			size_t sceneIndex;

		private:
			Scene* scene;
			Ref<bool> isLoaded = createRef<bool>(false);

			ManagedScene(const String& name, size_t sceneIndex, Scene* scene)
				: name(createRef<String>(name)), sceneIndex(sceneIndex), scene(scene)
			{
			}

			friend bool operator==(const ManagedScene& scene1, const ManagedScene& scene2) { 
				return scene1.scene == scene2.scene; }

		public:
			Scene* operator->() const { return scene; }
			Scene& operator*() const { return *scene; }

			friend class SceneManager;
		};

		SceneManager() = delete;
		SceneManager(const SceneManager& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;

		static const ManagedScene& getCurrentScene();
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

		static ManagedScene s_CurrentScene;
		static std::vector<ManagedScene> s_Scenes;

		friend class App;
	};

}
