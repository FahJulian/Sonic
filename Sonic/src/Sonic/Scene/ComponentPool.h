#pragma once
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <set>
#include "Sonic/Log/Log.h"
#include "Entity.h"
#include "ComponentView.h"

namespace Sonic {

    /**
    * A ComponentPool is a data structure that holds components of one type
    * and the entities the components are part of (Essentially a vector of 
    * EntityComponentPairs, but with custom memory management and without a
    * template, so the scene can store them by the components ComponentType 
    * IDs rather that by template
    */
    class ComponentPool
    {   
    private:
        /**
        * Constructs a NULL ComponentPool
        */
        ComponentPool();

        /**
        * Constructs a new ComponentPool (Should not be used, use ComponentPool::create
        * instead
        * 
        * @param componentSize The size in bytes of the component type
        * @param expectedMagnitude The expected magnitude of the component type (log() of
        *                          the expected amount of components that will exist
        *                          at the same time
        */
        ComponentPool(int componentSize, int expectedMagnitude);

        /**
        * @return Whether this object is null
        */
        bool IsNull();

        /**
        * Searches the data for the given entity. If ComponentPool::Has() has been called
        * previously, it begins searching at the index of the entity Has() has been called
        * with.
        *
        * @return The index of the given entity in m_Data
        */
        int IndexOf(Entity entity);

        /**
        * Increases the size of the buffer by m_ExpectedMagnitude. All Components and entities
        * get copied into the new buffer and the old one gets deleted
        */
        void IncreaseSize();

        /**
        * Destroys the buffer. This cant be done in the destructor because when the scene reallocates
        * its array of ComponentPools the destructors get called even though the ComponentPools
        * should not yet be destroyed
        */
        void Destroy();

        /**
        * Checks if the index of the entity is not -1. Stores the index of this entity so that if another
        * method that needs the index is called right after Has(), it can start by checking if the index
        * of the entity Has() was called with is fitting. Thus Calling Has() and then Get() has no extra
        * costs compared to only calling Get() and checking if the return value is NULL.
        *
        * @return whether or not this ComponentPool contains a component that is part of the given
        * entity
        * 
        * @param entity The entity to check against
        */
        bool Has(Entity entity);

        /**
        * Removes the component that is part of the given entity. That is quite costly if the entity is not
        * at the back of the buffer
        * 
        * @param entity The entity whos componed should be removed
        */
        void Remove(Entity entity);

        /**
        * Removes all the entities in the set. Starts with the highest Entity ID because the probability of 
        * that entities with a high ID being at the end of the buffer is higher
        */
        void Remove(const std::set<Entity>& entities);

        /**
        * Adds the given entity to the buffer. If the buffer is full it needs to be reallocated.
        * It is better to use the varargs version of this method to avoid copying.
        * 
        * @param entity The entity the added component is part of
        * @param component The component to add
        */
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

        /**
        * Adds the given entity to the buffer. If the buffer is full it needs to be reallocated.
        * 
        * @param entity The entity the added component is part of
        * @param args The arguments to pass to the components constructor
        */
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

        /**
        * Searches for the index of the given entity and returns the respective component. If Has() has been 
        * called previously with this entity, no searching is necessary. Thus Calling Has() and then Get() 
        * has no extra costs compared to only calling Get() and checking if the return value is NULL.
        *
        * @param entity The entity whos component should be returned
        * 
        * @return Pointer to the component of the given entity or NULL if none is found
        */
        template<typename Component>
        Component* Get(Entity entity)
        {
            int index = IndexOf(entity);

            SONIC_LOG_DEBUG_ASSERT(index != -1, "ComponentPool::Get(): Entity does not exist.")

            return reinterpret_cast<Component*>(m_Data + index * m_ElementSize + sizeof(Entity));
        }

        /**
        * @return A ComponentView object to iterate over the data of this ComponentPool. This 
        *         ComponentView should be destroyed before the next update phase because then
        *         the buffer of the ComponentPool could be moved resulting in the ComponentView
        *         pointing to freed memory
        */
        template<typename Component>
        ComponentView<Component> ToComponentView()
        {
            return { m_Data, m_Size };
        }

        /**
        * @return A ComponentPool with the right parameters for the given Component Type
        */
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
