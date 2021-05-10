#pragma once

#include <utility>

#include "sonic/debug/Log.h"

#include "sonic/util/Map.h"
#include "sonic/util/String.h"
#include "sonic/util/Optional.h"

#include "sonic/core/scene/Entity.h"

#include "sonic/core/scene/ecs/ComponentPool.h"
#include "sonic/core/scene/ecs/GroupView.h"

#if defined(SN_CLIENT_DYNAMIC)

	#define SN_RCR_DLL_PATH getClientDllPath()
	#include "sonic/util/rcr/RuntimeCodeReload.h"
	
	namespace sonic::client
	{
		extern String getClientDllPath();

		class Scene
		{
		public:
			using SceneLoadData = Map<String, Map<String, String>>;

			SN_RCR_MEMBER_FUNCTION_1_ARGS(void, load, SceneLoadData&, serializedScene);
			SN_RCR_MEMBER_FUNCTION_0_ARGS(void, init);
			SN_RCR_MEMBER_FUNCTION_1_ARGS(void, update, float, deltaTime);

			template<typename C>
			SN_RCR_MEMBER_FUNCTION_0_ARGS(ComponentPool<C>*, _getComponentPool, C);

			template<typename C>
			SN_RCR_MEMBER_FUNCTION_0_ARGS(void, _onComponentAdded, C);

			template<typename C>
			void addComponent(Entity entity, C component)
			{
				_getComponentPool<C>()->addComponent(entity, component);
				_onComponentAdded<C>();
			}
		};

		SN_RCR_FUNCTION_1_ARGS(Optional<Scene*>, createScene, const String&, sceneName);
		SN_RCR_FUNCTION_1_ARGS(void, initClientLog, Log*, mainLog);
	}

#else

	namespace sonic::client
	{
		extern class Scene
		{
			template<typename C>
			ComponentPool<C>* _getComponentPool();

			template<typename C>
			void _onComponentAdded();
		};
	}

#endif


namespace sonic
{
	class Scene : private client::Scene
	{

	};
}





//#if defined(SN_CLIENT_DYNAMIC)
//	
//	#define SN_CLIENT_API __declspec(dllexport)
//
//	extern sonic::String getClientDllPath();
//	#define SN_RCR_DLL_PATH getClientDllPath()
//	#include "sonic/util/rcr/RuntimeCodeReload.h"
//
//	namespace sonic
//	{
//		struct SceneData;
//
//		class Scene
//		{
//		public:
//			using SceneLoadData = Map<String, Map<String, String>>;
//
//			SN_RCR_MEMBER_FUNCTION_1_ARGS(void, load, SceneLoadData&, serializedScene);
//			SN_RCR_MEMBER_FUNCTION_0_ARGS(void, init);
//			SN_RCR_MEMBER_FUNCTION_1_ARGS(void, update, float, deltaTime);
//
//			template<typename C, typename... Args>
//			void addComponent(Entity entity, Args&&... args)
//			{
//				_getComponentPool<C>()->addComponent(entity, std::forward<Args>(args)...);
//				_onComponentAdded<C>();
//			}
//
//			template<typename C1, typename C2>
//			GroupView<C1, C2>& group()
//			{
//				static GroupView<C1, C2> group;	// Only a placeholder
//				return group;
//			}
//
//		private:
//			template<typename C>
//			SN_RCR_MEMBER_FUNCTION_0_ARGS(ComponentPool<C>*, _getComponentPool, C);
//
//			template<typename C>
//			SN_RCR_MEMBER_FUNCTION_0_ARGS(void, _onComponentAdded, C);
//		};
//
//		namespace client
//		{
//			SN_RCR_FUNCTION_1_ARGS(Optional<Scene*>, createScene, const String&, sceneName);
//			SN_RCR_FUNCTION_1_ARGS(void, initClientLog, Log*, mainLog);
//
//		} // namespace client
//
//	} // namespace sonic
//
//#else
//
//	#define SN_CLIENT_API
//
//	namespace sonic
//	{
//		struct SceneData;
//
//		class Scene
//		{
//		public:
//			extern void load(Map<String, Map<String, String>>& serializedScene);
//			extern void init();
//			extern void update(float deltaTime);
//
//			template<typename C, typename... Args>
//			void addComponent(Entity entity, Args&&... args)
//			{
//				getComponentPool<C>()->addComponent(entity, std::forward<Args>(args)...);
//				onComponentAdded<C>();
//			}
//
//			template<typename C1, typename C2>
//			GroupView<C1, C2>& group()
//			{
//				static GroupView<C1, C2> group;	// Only a placeholder
//				return group;
//			}
//
//			extern static Optional<Scene*> createScene(const String& sceneName);
//
//		protected:
//			extern Scene(String&& name, SceneData* data);
//
//			virtual void initComponentPools() = 0;
//			virtual void initSystems() = 0;
//			virtual void updateComponents(float deltaTime) = 0;
//			virtual void updateSystems(float deltaTime) = 0;
//
//		private:
//			template<typename C>
//			extern ComponentPool<C>* getComponentPool();
//
//			template<typename C>
//			extern void onComponentAdded();
//
//		private:
//			String mName;
//			SceneData* mData;
//		};
//
//	} // namespace sonic
//
//#endif
