#pragma once
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <set>
#include "Sonic/Log/Log.h"
#include "Entity.h"
#include "ComponentView.h"

namespace Sonic {

    class ComponentPool
    {   
    private:
        ComponentPool();
        ComponentPool(int componentSize, int expectedMagnitude);

        bool IsNull();

        int IndexOf(Entity entity);

        void IncreaseSize();

        void Destroy();

        bool Has(Entity entity);

        void Remove(Entity entity);

        void Remove(const std::set<Entity>& entities);

        template<typename Component>
        void Add(Entity entity, const Component& component)
        {
            SONIC_LOG_DEBUG_ASSERT(IndexOf(entity) == -1, "ComponentPool::Add(): Entity is already added.")

            if (m_Size == m_Capacity)
                IncreaseSize();
            
            int index = static_cast<int>(m_Size++);
            ::new(m_Data + index * m_ElementSize) Entity(entity);
            ::new(m_Data + index * m_ElementSize + sizeof(Entity)) Component(component);
        }

        template<typename Component, typename... Args>
        void Add(Entity entity, Args&&... args)
        {
            SONIC_LOG_DEBUG_ASSERT(IndexOf(entity) == -1, "ComponentPool::Add(): Entity is already added.")

            if (m_Size == m_Capacity)
                IncreaseSize();
            
            int index = static_cast<int>(m_Size++);
            ::new(m_Data + index * m_ElementSize) Entity(entity);
            #pragma warning(disable:4244)
            ::new(m_Data + index * m_ElementSize + sizeof(Entity)) Component(std::forward<Args>(args)...); 
            #pragma warning(default:4244)
        }

        template<typename Component>
        Component* Get(Entity entity)
        {
            int index = IndexOf(entity);

            SONIC_LOG_DEBUG_ASSERT(index != -1, "ComponentPool::Get(): Entity does not exist.")

            return reinterpret_cast<Component*>(m_Data + index * m_ElementSize + sizeof(Entity));
        }

        template<typename Component>
        ComponentView<Component> ToComponentView()
        {
            return { m_Data, m_Size };
        }

        template<typename Component>
        static ComponentPool create()
        {
            return ComponentPool(sizeof(Component), Component::getExpectedMagnitude());
        }

    private:
        size_t m_ElementSize;
        size_t m_ExpectedMagnitude;
        size_t m_Capacity;
        size_t m_Size;

        char* m_Data;

        int m_LastHasIndex;

        friend class Scene;
    };

}
