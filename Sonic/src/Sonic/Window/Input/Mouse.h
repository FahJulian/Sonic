#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "MouseButtons.h"

namespace Sonic {

    /**
    * Uninstantiable class that is responsible for turning glfw mosue events
    * into Sonic events and calling the scenes event dispatcher with that event
    */
    class Mouse
    {
    private:
        /**
        * Should be called with the data of the glfw cursorPos event. Converts it into a
        * sonic MouseMovedEvent / MouseDraggedEvent and dispatches it to the scene
        */
        static void cursorPosCallback(GLFWwindow* window, double posX, double posY);

        /**
        * Should be called with the data of the glfw mouse button event. Converts it into a
        * sonic MouseButtonPressedEvent / MouseButtonReleaedEvent and dispatches it to the scene
        */
        static void buttonCallback(GLFWwindow* window, int buttonID, int action, int mods);

        /**
        * Should be called with the data of the glfw scroll event. Converts it into a
        * sonic MouseScrolledEvent and dispatches it to the scene
        */
        static void scrollCallback(GLFWwindow* window, double scrollX, double scrollY);

    public:
        /**
        * Checks if the given button is currently pressed.
        *
        * @param button The button to check
        */
        static bool isButtonPressed(MouseButton button);

        static float getX();

        static float getY();

    public:

    private:
        friend class Window;
    };

}
