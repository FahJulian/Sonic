#pragma once
#include "Entity.h"

namespace Sonic {

    /**
    * Struct to Group ComponentData in ComponentViews and 
    * their Iterators
    */
    template<typename Component>
    struct EntityComponentPair
    {
        const Entity entity;
        Component component;
    };

}
