#pragma once
#include <stdint.h>

namespace Sonic {

    struct Direction2DComponent
    {
        enum class Direction : int8_t
        {
            Up = 2,
            Down = -2,
            Right = 1,
            Left = -1
        };

        Direction direction;
        Direction lastDirection;

        Direction2DComponent(Direction direction)
            : direction(direction), lastDirection(direction) {}
        Direction2DComponent(Direction direction, Direction lastDirection)
            : direction(direction), lastDirection(lastDirection) {}
    };

}
