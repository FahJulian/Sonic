#pragma once
#include "Sonic/Scene/Entity.h"
#include "Sonic/Window/Input/MouseButtons.h"
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

    struct MouseMovedEvent
    {
        float x;
        float y;
        float beforeX;
        float beforeY;
        float deltaX;
        float deltaY;

        MouseMovedEvent(float x, float y, float beforeX, float beforeY, float deltaX, float deltaY)
            : x(x), y(y), beforeX(beforeX), beforeY(beforeY), deltaX(deltaX), deltaY(deltaY)
        {
        }
    };

    struct MouseDraggedEvent
    {
        MouseButton button;
        float x;
        float y;
        float beforeX;
        float beforeY;
        float deltaX;
        float deltaY;

        MouseDraggedEvent(MouseButton button, float x, float y, float beforeX, float beforeY, float deltaX, float deltaY)
            : button(button), x(x), y(y), beforeX(beforeX), beforeY(beforeY), deltaX(deltaX), deltaY(deltaY)
        {
        }
    };

    struct MouseButtonPressedEvent
    {
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
    };

    struct WindowResizedEvent
    {
        float width;
        float height;

        WindowResizedEvent(float width, float height)
            : width(width), height(height)
        {
        }
    };

    template<typename Component>
    struct ComponentAddedEvent
    {
        Entity entity;

        ComponentAddedEvent(Entity entity)
            : entity(entity)
        {
        }
    };

    template<typename Component>
    struct ComponentRemovedEvent
    {
        Entity entity;

        ComponentRemovedEvent(Entity entity)
            : entity(entity)
        {
        }
    };
}
