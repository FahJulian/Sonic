#pragma once
#include "Sonic/Window/MouseButtons.h"
#include "Sonic/Window/Keys.h"
#include "Event.h"

namespace Sonic {

    struct KeyPressedEvent
    {
        SONIC_REGISTER_EVENT

        Key key;

        KeyPressedEvent(Key key)
            : key(key)
        {
        }
    };

    struct KeyReleasedEvent
    {
        SONIC_REGISTER_EVENT

        Key key;

        KeyReleasedEvent(Key key)
            : key(key)
        {
        }
    };

    struct MouseMovedEvent
    {
        SONIC_REGISTER_EVENT

        float x;
        float y;
        float deltaX;
        float deltaY;

        MouseMovedEvent(float x, float y, float deltaX, float deltaY)
            : x(x), y(y), deltaX(deltaX), deltaY(deltaY)
        {
        }
    };

    struct MouseDraggedEvent
    {
        SONIC_REGISTER_EVENT

        MouseButton button;
        float x;
        float y;
        float deltaX;
        float deltaY;

        MouseDraggedEvent(MouseButton button, float x, float y, float deltaX, float deltaY)
            : button(button), x(x), y(y), deltaX(deltaX), deltaY(deltaY)
        {
        }
    };

    struct MouseButtonPressedEvent
    {
        SONIC_REGISTER_EVENT

        MouseButton button;
        float x;
        float y;

        MouseButtonPressedEvent(MouseButton button, float x, float y)
            : button(button), x(x), y(y)
        {
        }
    };

    struct MouseButtonReleasedEvent
    {
        SONIC_REGISTER_EVENT

        MouseButton button;
        float x;
        float y;

        MouseButtonReleasedEvent(MouseButton button, float x, float y)
            : button(button), x(x), y(y)
        {
        }
    };

    struct MouseScrolledEvent
    {
        SONIC_REGISTER_EVENT

        float x;
        float y;
        float scrollX;
        float scrollY;

        MouseScrolledEvent(float x, float y, float scrollX, float scrollY)
            : x(x), y(y), scrollX(scrollX), scrollY(scrollY)
        {
        }
    };

    struct WindowClosedEvent
    {
        SONIC_REGISTER_EVENT

    };

    struct WindowResizedEvent
    {
        SONIC_REGISTER_EVENT

        int width;
        int height;

        WindowResizedEvent(int width, int height)
            : width(width), height(height)
        {
        }
    };

}
