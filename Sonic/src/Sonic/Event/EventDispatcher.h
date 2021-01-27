#pragma once
#include <functional>
#include <vector>
#include <unordered_map>

namespace Sonic {

	template<typename Event>
	using EventListener = std::function<void(const Event&)>;

	class EventDispatcher
	{
	public:
		template<typename Event>
		void AddListener(EventListener<Event> listener)
		{
			GetListeners<Event>().push_back(listener);
		}

		template<typename F, typename Event>
		void AddListener(F* key, void(F::* listenerMethod)(const Event&))
		{
			EventListener<Event> listener = [key, listenerMethod](const Event& e) { (key->*listenerMethod)(e); };
			GetListeners<Event>().push_back(listener);
		}

		template<typename F, typename Event>
		void AddKeyedListener(F* key, void(F::* listenerMethod)(const Event&))
		{
			EventListener<Event> listener = [key, listenerMethod](const Event& e) { (key->*listenerMethod)(e); };
			auto& listeners = GetListeners<Event>();
			listeners.push_back(listener);
			GetKeys<Event>()[reinterpret_cast<intptr_t>(key)] = static_cast<unsigned int>(listeners.size()) - 1;
		}

		template<typename Event, typename F>
		void RemoveKeyedListener(F* object)
		{
			auto& keys = GetKeys<Event>();
			auto& listeners = GetListeners<Event>();

			intptr_t key = reinterpret_cast<intptr_t>(object);
			unsigned int index = keys[key];
			keys.erase(key);
			listeners.erase(listeners.begin() + index);

			for (auto& [_, value] : keys)
				if (value > index)
					value--;
		}

		template<typename Event>
		void DispatchEvent(const Event& e)
		{
			for (EventListener<Event>& listener : GetListeners<Event>())
				listener(e);
		}

	private:
		template<typename Event>
		std::vector<EventListener<Event>>& GetListeners()
		{
			static std::unordered_map<EventDispatcher*, std::vector<EventListener<Event>>> listeners;
			return listeners[this];
		}

		template<typename Event>
		std::unordered_map<intptr_t, unsigned int>& GetKeys()
		{
			static std::unordered_map<EventDispatcher*, std::unordered_map<intptr_t, unsigned int>> keys;
			return keys[this];
		}
	};

}