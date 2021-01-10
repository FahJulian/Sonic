#pragma once
#include "Entity.h"

namespace Sonic {

    template<typename Component>
    struct EntityComponentPair
    {
        const Entity entity;
        Component component;
    };

}
