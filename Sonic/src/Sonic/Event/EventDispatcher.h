#pragma once
#include <vector>

namespace Sonic {

    template<typename E>
    class EventDispatcher
    {
    private:
        using EventListenerFunction = void(*)(const E&);

        struct EventListenerMethod
        {
            void* const object;
            void(*function)(void* const, const E&);

            EventListenerMethod(void* const object, void(*function)(void* const, const E&))
                : object(object), function(function)
            {
            }

            void operator()(const E& event)
            {
                function(object, event);
            }
        };

    public:
        void AddListener(const EventListenerFunction& listener)
        {
            m_Functions.emplace_back(listener);
        }

        template<typename F, void(F::*M)(const E&)>
        void AddListener(F* const object)
        {
            m_Methods.emplace_back(
                object,
                [](void* const p, const E& event){ 
                    (static_cast<F* const>(p)->*M)(event); 
                }
            );
        }

        void Dispatch(const E& event)
        {
            for (int i = 0; i < m_Methods.size(); i++)
                m_Methods.at(i)(event);

            for (int i = 0; i < m_Functions.size(); i++)
                m_Functions.at(i)(event);
        }

    private:
        std::vector<EventListenerMethod> m_Methods;
        std::vector<EventListenerFunction> m_Functions;
    };

}
