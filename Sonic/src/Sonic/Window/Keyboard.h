#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Sonic/Event/Events.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Keys.h"

namespace Sonic {

    /**
    * Uninstantiable class that is responsible for turning glfw keyboard events
    * into Sonic events and calling the scenes event dispatcher with that event
    */
    class Keyboard
    {
    private:
        Keyboard() = delete;
        Keyboard(const Keyboard& other) = delete;

        /**
        * Should be called with the data of the glfw key event. Converts it into a
        * sonic KeyPressedEvent or KeyReleaedEvent and dispatches it to the scene
        */
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    public:
        /**
        * Checks if the given key is currently pressed.
        * 
        * @param key The key to check
        */
        static bool isKeyPressed(Key key);

    private:
        friend class Window;
    };

}
