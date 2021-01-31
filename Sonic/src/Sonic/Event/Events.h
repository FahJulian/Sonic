#pragma once
#include "Sonic/Scene/EntityID.h"
#include "Sonic/Window/MouseButtons.h"
#include "Sonic/Window/Keys.h"

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
        float deltaX;
        float deltaY;

        MouseMovedEvent(float x, float y, float deltaX, float deltaY)
            : x(x), y(y), deltaX(deltaX), deltaY(deltaY)
        {
        }
    };

    struct MouseDraggedEvent
    {
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
        int width;
        int height;

        WindowResizedEvent(int width, int height)
            : width(width), height(height)
        {
        }
    };

    template<typename Component>
    struct ComponentAddedEvent
    {
        EntityID entity;

        ComponentAddedEvent(EntityID entity)
            : entity(entity)
        {
        }
    };

    template<typename Component>
    struct ComponentRemovedEvent
    {
        EntityID entity;

        ComponentRemovedEvent(EntityID entity)
            : entity(entity)
        {
        }
    };
}
