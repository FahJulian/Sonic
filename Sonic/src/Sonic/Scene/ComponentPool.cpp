#include "ComponentPool.h"

namespace Sonic {

	ComponentPool::ComponentPool()
        : m_ElementSize(0), m_ExpectedMagnitude(0), m_Capacity(0), m_Size(0), 
        m_Data(nullptr), m_LastHasIndex(0)
    {
    }

    ComponentPool::ComponentPool(int componentSize, int expectedMagnitude)
        : m_ElementSize(componentSize + sizeof(Entity)),
        m_ExpectedMagnitude(static_cast<int>(pow(10, expectedMagnitude))),
        m_Capacity(m_ExpectedMagnitude), m_Size(0), m_Data(new char[m_Capacity * m_ElementSize]),
        m_LastHasIndex(0)
    {
    }

    bool ComponentPool::IsNull()
    {
        return m_Data == nullptr;
    }

    bool ComponentPool::Has(Entity entity)
    {
        m_LastHasIndex = IndexOf(entity);
        return m_LastHasIndex != -1;
    }

    void ComponentPool::Remove(Entity entity)
    {
        int index = IndexOf(entity);

        SONIC_LOG_DEBUG_ASSERT(index != -1, "ComponentPool::Remove(): Entity is already removed.");

        if (index != m_Size)
            std::copy(m_Data + (static_cast<size_t>(index) + 1) * m_ElementSize, m_Data + m_Size * m_ElementSize, m_Data + index * m_ElementSize);
        m_Size--;
    }

    void ComponentPool::Remove(const std::set<Entity>& entities)
    {
        using Iterator = std::set<Entity>::reverse_iterator;
        for (Iterator i = entities.rbegin(); i != entities.rend(); i++)
            if (Has(*i))
                Remove(*i);
    }

    void ComponentPool::IncreaseSize()
    {
        m_Capacity += m_ExpectedMagnitude;
        char* newData = new char[m_Capacity * m_ElementSize];
        std::copy(m_Data, m_Data + m_Size * m_ElementSize, newData);

        delete[] m_Data;
        m_Data = newData;
    }

    void ComponentPool::Destroy()
    {
        delete[] m_Data;
    }

    int ComponentPool::IndexOf(Entity entity)
    {
        if (m_LastHasIndex != -1 && *reinterpret_cast<Entity*>(m_Data + m_LastHasIndex * m_ElementSize) == entity)
            return m_LastHasIndex;

        for (unsigned int i = 0; i < m_Size; i++)
            if (*reinterpret_cast<Entity*>(m_Data + i * m_ElementSize) == entity)
                return i;

        return -1;
    }
}
