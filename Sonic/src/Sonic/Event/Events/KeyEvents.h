#pragma once
#include "Sonic/Window/Input/Keys.h"

namespace Sonic {

    struct KeyPressedEvent
    {
        Key key;

        KeyPressedEvent(Key key)
            : key(key)
        {
        }
    };

    struct KeyReleasedEvent
    {
        Key key;

        KeyReleasedEvent(Key key)
            : key(key)
        {
        }
    };

}