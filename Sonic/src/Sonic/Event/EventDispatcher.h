#pragma once
#include <vector>
#include <functional>

static const size_t VECTOR_RESERVE_STEP = 10;

namespace Sonic {

    /**
    * An EventDispatcher holds member methods and functions that get called
    * when an Event of the given type is dispatched
    */
    template<typename Event>
    class EventDispatcher
    {
    public:
        /**
        * A function that gets called when an Event of the given type is dispatched.
        */
        using EventListener = std::function<void(const Event&)>;

    public:
        /**
        * Constructs a new EventDispatcher
        */
        EventDispatcher()
        {
            m_Listeners.reserve(VECTOR_RESERVE_STEP);
        }

        /**
        * Adds a listener to the EventDispatcher. This method should be used to add 
        * functions, not methods.
        * 
        * @param listener Pointer to the function
        */
        void AddListener(EventListener listener)
        {
            m_Listeners.emplace_back(listener);

            if (m_Listeners.capacity() == m_Listeners.size())
                m_Listeners.reserve(m_Listeners.capacity() + VECTOR_RESERVE_STEP);
        }

        /**
        * Adds a listener to the EventDispatcher. This method should be used to add
        * methods, not functions.
        * 
        * @param obj Pointer to the object of the method
        * @param method Pointer to the method
        */
        template<typename F>
        void AddListener(F* const obj, void(F::*method)(const Event&))
        {
            EventListener listener = [obj, method](const Event& e) { (obj->*method)(e); };
            AddListener(listener);
        }

        /**
        * Calls all registered event listeners with the given event.
        * @param event The event to dispatch
        */
        void Dispatch(const Event& event)
        {
            for (EventListener listener : m_Listeners)
                listener(event);
        }

    private:
        std::vector<std::function<void(const Event&)>> m_Listeners;
    };

}
