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

            Iterator(pointer ptr)
                : m_Pointer(ptr)
            {
            }

            reference operator*() { return *m_Pointer; }
            pointer operator->() { return m_Pointer; }

            Iterator& operator++() { m_Pointer++; return *this; }
            Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

            friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_Pointer == b.m_Pointer; }
            friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_Pointer != b.m_Pointer; } 

        private:
            pointer m_Pointer;
        };

    private:
        using Pair = EntityComponentPair<Component>;

        ComponentView(char* data, size_t size)
            : m_Data(data), m_Size(size)
        {
        }
    
    public:
        Iterator begin() { return Iterator(reinterpret_cast<Pair*>(m_Data)); }
        Iterator end() { return Iterator(reinterpret_cast<Pair*>(m_Data + m_Size * sizeof(Pair))); }

        void ForEach(std::function<void(const Entity, Component&)> function)
        {
            for (auto& [entity, component] : *this)
                function(entity, component);
        }

    private:
        char* m_Data;
        const size_t m_Size;

        friend class ComponentPool;
    };

}
