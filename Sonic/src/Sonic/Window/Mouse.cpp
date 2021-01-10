#include "Mouse.h"

struct MouseData
{
    float m_X = 0.0f;
    float m_Y = 0.0f;
    bool m_PressedButtons[GLFW_MOUSE_BUTTON_LAST] = { false };
};

static MouseData s_Data;


namespace Sonic {

    EventDispatcher<MouseMovedEvent> Mouse::onMouseMoved;
    EventDispatcher<MouseDraggedEvent> Mouse::onMouseDragged;
    EventDispatcher<MouseButtonPressedEvent> Mouse::onMouseButtonPressed;
    EventDispatcher<MouseButtonReleasedEvent> Mouse::onMouseButtonReleased;
    EventDispatcher<MouseScrolledEvent> Mouse::onMouseScrolled;

    void Mouse::init()
    {
        s_Data = MouseData();

        Mouse::onMouseMoved = EventDispatcher<MouseMovedEvent>();
        Mouse::onMouseDragged = EventDispatcher<MouseDraggedEvent>();
        Mouse::onMouseButtonPressed = EventDispatcher<MouseButtonPressedEvent>();
        Mouse::onMouseButtonReleased = EventDispatcher<MouseButtonReleasedEvent>();
        Mouse::onMouseScrolled = EventDispatcher<MouseScrolledEvent>();
    }

    void Mouse::cursorPosCallback(GLFWwindow* window, double posX, double posY)
    {
        int _, height;
        glfwGetWindowSize(window, &_, &height);

        float x = static_cast<float>(posX);
        float y = static_cast<float>(height) - static_cast<float>(posY);

        onMouseMoved.Dispatch(MouseMovedEvent(x, y, x - s_Data.m_X, y - s_Data.m_Y));

        for (int button = 0; button < GLFW_MOUSE_BUTTON_LAST; button++)
            if (s_Data.m_PressedButtons[button])
                onMouseDragged.Dispatch(MouseDraggedEvent(button, x, y, x - s_Data.m_X, y - s_Data.m_Y));

        s_Data.m_X = x;
        s_Data.m_Y = y;
    }

    void Mouse::buttonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            s_Data.m_PressedButtons[button] = true;
            onMouseButtonPressed.Dispatch(MouseButtonPressedEvent(button, s_Data.m_X, s_Data.m_Y));
        }
        else if (action == GLFW_RELEASE)
        {
            s_Data.m_PressedButtons[button] = false;
            onMouseButtonReleased.Dispatch(MouseButtonReleasedEvent(button, s_Data.m_X, s_Data.m_Y));
        }
    }

    void Mouse::scrollCallback(GLFWwindow* window, double scrollX, double scrollY)
    {
        onMouseScrolled.Dispatch(MouseScrolledEvent(s_Data.m_X, s_Data.m_Y, static_cast<float>(scrollX), static_cast<float>(scrollY)));
    }

    bool Mouse::isButtonPressed(MouseButton button) 
    { 
        return s_Data.m_PressedButtons[button]; 
    }
    
    float Mouse::getX() 
    { 
        return s_Data.m_X; 
    }

    float Mouse::getY() 
    { 
        return s_Data.m_Y; 
    }
}