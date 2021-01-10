#pragma once
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <functional>
#include "Entity.h"
#include "EntityComponentPair.h"
#include "../Event/EventDispatcher.h"
#include "../Event/Events.h"

namespace Sonic {

    template<typename Component>
    class ComponentView
    {   
    public:
        struct Iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = EntityComponentPair<Component>;
            using pointer = EntityComponentPair<Component>*;
            using reference = EntityComponentPair<Component>&;

            Iterator(pointer ptr, int& componentPoolActiveIterators)
                : m_Pointer(ptr), m_ComponentPoolActiveIterators(componentPoolActiveIterators)
            {
                m_ComponentPoolActiveIterators++;
            }

            reference operator*() { return *m_Pointer; }
            pointer operator->() { return m_Pointer; }

            Iterator& operator++() { m_Pointer++; return *this; }
            Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

            friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_Pointer == b.m_Pointer; }
            friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_Pointer != b.m_Pointer; } 

            ~Iterator()
            {
                m_ComponentPoolActiveIterators--;
            }

        private:
            pointer m_Pointer;
            int& m_ComponentPoolActiveIterators;
        };

    private:
        ComponentView(char* data, size_t size, size_t elementSize, int& componentPoolActiveIterators)
            : m_Data(data), m_Size(size), m_ElementSize(elementSize), 
            m_ComponentPoolActiveIterators(componentPoolActiveIterators)
        {
        }
    
    public:
        EntityComponentPair<Component> At(int index)
        {
            return { *reinterpret_cast<Entity*>(m_Data + index * m_ElementSize),
                reinterpret_cast<Component*>(m_Data + index * m_ElementSize + sizeof(Entity)) };
        }

        EntityComponentPair<Component> operator[](int index) { return At(index); }

        size_t Size() { return m_Size; }

        Iterator begin() 
        {
            return Iterator(
                reinterpret_cast<EntityComponentPair<Component>*>(m_Data),
                m_ComponentPoolActiveIterators
            );
        }

        Iterator end() 
        { 
            return Iterator(
                reinterpret_cast<EntityComponentPair<Component>*>(m_Data + m_Size * sizeof(EntityComponentPair<Component>)),
                m_ComponentPoolActiveIterators
            ); 
        }

        void ForEach(std::function<void(const Entity, Component&)> function)
        {
            for (auto& [entity, component] : *this)
                function(entity, component);
        }

    private:
        char* m_Data;
        const size_t m_ElementSize; 
        const size_t m_Size;
        int& m_ComponentPoolActiveIterators;

        friend class ComponentPool;
    };

}
