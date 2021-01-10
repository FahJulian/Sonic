#pragma once

#define SONIC_REGISTER_COMPONENT(expectedMagnitude) \
    static Sonic::ComponentType getComponentType() \
    { \
        static const Sonic::ComponentType type = Sonic::getNextComponentType(); \
        return type; \
    } \
    static int getExpectedMagnitude() \
    { \
        return expectedMagnitude; \
    }

namespace Sonic {

    using ComponentType = unsigned short;

    ComponentType getNextComponentType();

}   
