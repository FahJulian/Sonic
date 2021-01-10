#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../Event/EventDispatcher.h"
#include "../Event/Events.h"
#include "MouseButtons.h"

namespace Sonic {

    class Mouse
    {
    private:
        static void init();
        static void cursorPosCallback(GLFWwindow* window, double posX, double posY);
        static void buttonCallback(GLFWwindow* window, int buttonID, int action, int mods);
        static void scrollCallback(GLFWwindow* window, double scrollX, double scrollY);
    public:
        static bool isButtonPressed(MouseButton button);
        static float getX();
        static float getY();

    public:
        static EventDispatcher<MouseMovedEvent> onMouseMoved;
        static EventDispatcher<MouseDraggedEvent> onMouseDragged;
        static EventDispatcher<MouseButtonPressedEvent> onMouseButtonPressed;
        static EventDispatcher<MouseButtonReleasedEvent> onMouseButtonReleased;
        static EventDispatcher<MouseScrolledEvent> onMouseScrolled;

    private:
        friend class Window;

    public:
        enum class Button : int
        {
            Button_Left = GLFW_MOUSE_BUTTON_LEFT,
            Button_Right = GLFW_MOUSE_BUTTON_RIGHT,
            Button_Middle = GLFW_MOUSE_BUTTON_MIDDLE,
            Button_4 = GLFW_MOUSE_BUTTON_4,
            Button_5 = GLFW_MOUSE_BUTTON_5,
            Button_6 = GLFW_MOUSE_BUTTON_6,
            Button_7 = GLFW_MOUSE_BUTTON_7,
            Button_8 = GLFW_MOUSE_BUTTON_8
        };
    };

}
