#pragma once
#include <unordered_map>
#include <unordered_set>

namespace Sonic {

	namespace GenericContainer {

		using Key = uint32_t;

		Key nextKey();

		template<typename T, typename... Args>
		T* GetOrAdd(Key key, const Args&... args)
		{
			static std::unordered_map<Key, T*> instances;

			auto it = instances.find(key);
			if (it == instances.end())
			{
				instances.emplace(key, new T((args)...));
				it--;
			}

			return it->second;
		}

		template<typename Base>
		const std::unordered_set<Base*>& GetAll(Key key)
		{
			static std::unordered_map<Key, std::unordered_set<Base*>> allInstances;
			return allInstances[key];
		}

		template<typename T, typename Base, typename... Args>
		T* GetOrAddWithBase(Key key, const Args&... args)
		{
			static std::unordered_map<Key, T*> instances;

			auto it = instances.find(key);
			if (it == instances.end())
			{
				T* newInstance = new T((args)...);
				instances.emplace(key, newInstance);
				const_cast<std::unordered_set<Base*>&>(GetAll<Base>(key)).insert(dynamic_cast<Base*>(newInstance));
				it--;
			}

			return it->second;
		}

	}

}
