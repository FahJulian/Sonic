#include "Component.h"

static Sonic::ComponentType nextComponentType = 1;

namespace Sonic {

    ComponentType getNextComponentType()
    {
        return nextComponentType++;
    }

}
