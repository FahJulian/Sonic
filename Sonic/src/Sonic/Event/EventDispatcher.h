#pragma once
#include <vector>
#include <functional>

static const size_t VECTOR_RESERVE_STEP = 10;

namespace Sonic {

    template<typename Event>
    class EventDispatcher
    {
    public:
        using EventListener = std::function<void(const Event&)>;

    public:
        EventDispatcher()
        {
            m_Listeners.reserve(VECTOR_RESERVE_STEP);
        }

        void AddListener(EventListener listener)
        {
            m_Listeners.emplace_back(listener);

            if (m_Listeners.capacity() == m_Listeners.size())
                m_Listeners.reserve(m_Listeners.capacity() + VECTOR_RESERVE_STEP);
        }

        template<typename F>
        void AddListener(F* const obj, void(F::*method)(const Event&))
        {
            EventListener listener = [obj, method](const Event& e) { (obj->*method)(e); };
            AddListener(listener);
        }

        void Dispatch(const Event& event)
        {
            for (EventListener listener : m_Listeners)
                listener(event);
        }

    private:
        std::vector<std::function<void(const Event&)>> m_Listeners;
    };

}
