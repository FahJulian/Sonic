#pragma once
#include <unordered_map>
#include <unordered_set>

namespace Sonic {

	namespace GenericContainer {

		template<typename T, typename F, typename... Args>
		T* GetOrAdd(F* key, Args&&... args)
		{
			static std::unordered_map<F*, T*> instances;

			auto it = instances.find(key);
			if (it == instances.end())
			{
				instances.emplace(key, new T(std::forward<Args>(args)...));
				it--;
			}

			return it->second;
		}

		template<typename Base, typename F>
		const std::unordered_set<Base*>& GetAll(F* key)
		{
			static std::unordered_map<F*, std::unordered_set<Base*>> allInstances;
			return allInstances[key];
		}

		template<typename T, typename Base, typename F, typename... Args>
		T* GetOrAddWithBase(F* key, Args&&... args)
		{
			static std::unordered_map<F*, T*> instances;

			auto it = instances.find(key);
			if (it == instances.end())
			{
				T* newInstance = new T(std::forward<Args>(args)...);
				instances.emplace(key, newInstance);
				const_cast<std::unordered_set<Base*>&>(GetAll<Base>(key)).insert(dynamic_cast<Base*>(newInstance));
				it--;
			}

			return it->second;
		}
	}

	//>	Snake.exe!Sonic::GenericContainer::GetOrAddWithBase<Sonic::ComponentPool<Sonic::UIRendererComponent>,Sonic::BaseComponentPool,Sonic::Scene,Sonic::Scene *>(Sonic::Scene * key, Sonic::Scene * && <args_0>) Line 36	C++
	//>	Snake.exe!Sonic::GenericContainer::GetOrAddWithBase<Sonic::ComponentPool<Sonic::UIRendererComponent>,Sonic::BaseComponentPool,Sonic::Scene,Sonic::Scene * &>(Sonic::Scene * key, Sonic::Scene * & <args_0>) Line 36	C++


}
