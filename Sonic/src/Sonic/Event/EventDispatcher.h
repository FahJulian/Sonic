#pragma once
#include <vector>
#include <functional>
#include <memory>

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
        * functions, not methods. This adds a removable listener, meaning that the
        * returned int ptr can later be used to remove this listener.
        * 
        * @param listener Pointer to the function
        * 
        * @return pointer to the index of the function within this EventDispatcher. Can be used
        *         to remove the function later.
        */
        std::shared_ptr<int> AddRemovableListener(EventListener listener)
        {
            m_Listeners.emplace_back(listener);

            if (m_Listeners.capacity() == m_Listeners.size())
                m_Listeners.reserve(m_Listeners.capacity() + VECTOR_RESERVE_STEP);

            std::shared_ptr<int> indexPtr = std::make_shared<int>(m_Listeners.size() - 1);
            m_IndexRefs.push_back(indexPtr);
            return indexPtr;
        }
            
        /**
        * Adds a listener to the EventDispatcher. This method should be used to add
        * methods, not functions. This adds a removable listener, meaning that the
        * returned int ptr can later be used to remove this listener.
        * 
        * @param obj Pointer to the object of the method
        * @param method Pointer to the method
        * 
        * @return pointer to the index of the method within this EventDispatcher. Can be used
        *         to remove the method later.
        */
        template<typename F>
        std::shared_ptr<int> AddRemovableListener(F* const obj, void(F::*method)(const Event&))
        {
            EventListener listener = [obj, method](const Event& e) { (obj->*method)(e); };
            return AddListener(listener);
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
        void AddListener(F* const obj, void(F::* method)(const Event&))
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

        /**
        * Removes the listener at the specified index
        * 
        * @param index The index of the listener to remove
        */
        void Remove(const std::shared_ptr<int>& index)
        {
            m_Listeners.erase(m_Listeners.begin() + *index);

            int indexRefRemoveIdx = 0;
            for (int i = 0; i < m_IndexRefs.size(); i++)
            {
                if (*(m_IndexRefs.at(i)) == *index)
                    indexRefRemoveIdx = i;
                else if (*(m_IndexRefs.at(i)) > *index)
                    (*(m_IndexRefs.at(i)))--;
            }

            m_IndexRefs.erase(m_IndexRefs.begin() + indexRefRemoveIdx);
        }

    private:
        std::vector<std::function<void(const Event&)>> m_Listeners;
        std::vector<std::shared_ptr<int>> m_IndexRefs;
    };

}
