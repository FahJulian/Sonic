#pragma once
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "../Log/Log.h"
#include "Entity.h"
#include "ComponentView.h"
#include "../Event/EventDispatcher.h"
#include "../Event/Events.h"

namespace Sonic {

    class ComponentPool
    {   
    private:
        ComponentPool()
            : m_ElementSize(0), m_ExpectedMagnitude(0), m_Capacity(0), m_Size(0), m_Data(nullptr), m_ActiveIterators(0)
        {
        }

        ComponentPool(int componentSize, int expectedMagnitude)
            : m_ElementSize(componentSize + sizeof(Entity)),
            m_ExpectedMagnitude(static_cast<int>(pow(10, expectedMagnitude))),
            m_Capacity(m_ExpectedMagnitude), m_Size(0), m_Data(new char[m_Capacity * m_ElementSize]),
            m_ActiveIterators(0)
        {
        }

        template<typename Component>
        static ComponentPool create()
        {
            return ComponentPool(sizeof(Component), Component::getExpectedMagnitude());
        }

        bool IsNull()
        {
            return m_Data == nullptr;
        }

        template<typename Component>
        ComponentView<Component> ToComponentView()
        {
            return { m_Data, m_Size, m_ElementSize, m_ActiveIterators };
        }

    public:
        template<typename Component>
        void Add(Entity entity, const Component& component)
        {
            #ifdef SONIC_DEBUG
            if (IndexOf(entity) != -1)
            {
                SONIC_LOG_DEBUG("ComponentPool::Add(): Entity is already added.")
                return;
            }
            #endif

            if (m_Size == m_Capacity)
                IncreaseSize();
            
            int index = static_cast<int>(m_Size++);
            ::new(m_Data + index * m_ElementSize) Entity(entity);
            ::new(m_Data + index * m_ElementSize + sizeof(Entity)) Component(component);
        }

        template<typename Component, typename... Args>
        void Add(Entity entity, Args&&... args)
        {
            #ifdef SONIC_DEBUG
            if (IndexOf(entity) != -1)
            {
                SONIC_LOG_DEBUG("ComponentPool::Add(): Entity is already added.")
                return;
            }
            #endif

            if (m_Size == m_Capacity)
                IncreaseSize();
            
            int index = static_cast<int>(m_Size++);
            ::new(m_Data + index * m_ElementSize) Entity(entity);
            #pragma warning(disable:4244)
            ::new(m_Data + index * m_ElementSize + sizeof(Entity)) Component(std::forward<Args>(args)...); 
            #pragma warning(default:4244)
        }

        bool Has(Entity entity)
        {
            return IndexOf(entity) != -1;
        }

        template<typename Component>
        Component* Get(Entity entity)
        {
            int index = IndexOf(entity);

            #ifdef SONIC_DEBUG
            if (index == -1)
            {
                SONIC_LOG_DEBUG("ComponentPool::Get(): Entity does not exist.")
                return  nullptr;
            }
            #endif

            return reinterpret_cast<Component*>(m_Data + index * m_ElementSize + sizeof(Entity));
        }

        void Remove(Entity entity)
        {
            int index = IndexOf(entity);

            #ifdef SONIC_DEBUG
            if (index == -1)
            {
                SONIC_LOG_DEBUG("ComponentPool::Remove(): Entity is already removed.")
                return;
            }
            #endif

            if (index != m_Size)
                DecreaseSize(index);
            m_Size--;
        }

    private:
        int IndexOf(Entity entity)
        {
            for (unsigned int i = 0; i < m_Size; i++)
                if (*reinterpret_cast<Entity*>(m_Data + i * m_ElementSize) == entity)
                    return i;

            return -1;
        }

        void IncreaseSize()
        {
            m_Capacity += m_ExpectedMagnitude;
            char* newData = new char[m_Capacity * m_ElementSize];
            std::copy(m_Data, m_Data + m_Size * m_ElementSize, newData);

            delete[] m_Data;
            m_Data = newData;
        }

        void DecreaseSize(size_t removedIndex)
        {
            m_Capacity = (m_Size / m_ExpectedMagnitude + 1) * m_ExpectedMagnitude;
            char* newData = new char[m_Capacity * m_ElementSize];
            std::copy(m_Data, m_Data + removedIndex * m_ElementSize, newData);
            std::copy(m_Data + (removedIndex + 1) * m_ElementSize, m_Data + m_Size * m_ElementSize, newData + removedIndex * m_ElementSize);

            delete[] m_Data;
            m_Data = newData;
        }

        void Destroy()
        {
            delete[] m_Data;
        }

    private:
        size_t m_ElementSize;
        size_t m_ExpectedMagnitude;
        size_t m_Capacity;
        size_t m_Size;

        char* m_Data;

        int m_ActiveIterators;

        friend class Scene;
    };

}
