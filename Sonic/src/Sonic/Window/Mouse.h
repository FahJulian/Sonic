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
        static void cursorPosCallback(GLFWwindow* window, double posX, double posY);
        static void buttonCallback(GLFWwindow* window, int buttonID, int action, int mods);
        static void scrollCallback(GLFWwindow* window, double scrollX, double scrollY);
    public:
        static bool isButtonPressed(MouseButton button);
        static float getX();
        static float getY();

    public:

    private:
        friend class Window;
    };

}
