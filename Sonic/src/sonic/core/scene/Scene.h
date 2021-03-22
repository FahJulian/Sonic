#pragma once

#include <utility>

#include "sonic/debug/Log.h"

#include "sonic/util/Map.h"
#include "sonic/util/String.h"
#include "sonic/util/Optional.h"

#include "sonic/core/scene/Entity.h"

#include "sonic/core/scene/ecs/ComponentPool.h"
#include "sonic/core/scene/ecs/GroupView.h"

#if defined(SN_CLIENT_LINK_STATIC)
#if defined(SN_CLIENT_LINK_IMPORT)
#define SN_CLIENT_API extern
#elif defined(SN_CLIENT_LINK_EXPORT)
#define SN_CLIENT_API
#else
#error Client linking method must be specified.
#endif
#elif defined (SN_CLIENT_LINK_DYNAMIC)
#if defined(SN_CLIENT_LINK_IMPORT)
#define SN_CLIENT_API __declspec(dllimport)
#elif defined(SN_CLIENT_LINK_EXPORT)
#define SN_CLIENT_API __declspec(dllexport)
#else
#error Client linking method must be specified.
#endif
#else
#error Client linking method must be specified.
#endif

#if defined(SN_CLIENT_LINK_DYNAMIC)
namespace sonic::client
{
	SN_CLIENT_API void initClientLog(Log* mainLog);
}
#endif

namespace sonic
{
	struct SceneData;

	class Scene
	{
	public:
		SN_CLIENT_API void load(Map<String, Map<String, String>>& serializedScene);
		SN_CLIENT_API void init();
		SN_CLIENT_API void update(float deltaTime);

		template<typename C, typename... Args>
		void addComponent(Entity entity, Args&&... args)
		{
			getComponentPool<C>()->addComponent(entity, std::forward<Args>(args)...);
			onComponentAdded<C>();
		}

		template<typename C1, typename C2>
		GroupView<C1, C2>& group()
		{
			static GroupView<C1, C2> group;	// Only a placeholder
			return group;
		}

		SN_CLIENT_API static Optional<Scene*> createScene(const String& sceneName);

	protected:
		SN_CLIENT_API Scene(String&& name, SceneData* data);

		virtual void initComponentPools() = 0;
		virtual void initSystems() = 0;
		virtual void updateComponents(float deltaTime) = 0;
		virtual void updateSystems(float deltaTime) = 0;

	private:
		template<typename C>
		SN_CLIENT_API ComponentPool<C>* getComponentPool();

		template<typename C>
		SN_CLIENT_API void onComponentAdded();

	private:
		String mName;
		SceneData* mData;
	};

} // namespace sonic
