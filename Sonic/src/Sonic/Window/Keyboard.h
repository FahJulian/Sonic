#pragma once
#include <GLFW/glfw3.h>
#include "../Event/Events.h"
#include "../Event/EventDispatcher.h"
#include "Keys.h"

namespace Sonic {

    class Keyboard
    {
    private:
        static void init();
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    public:
        static bool isKeyPressed(Key key);

    public:
        static EventDispatcher<KeyPressedEvent> onKeyPressed;
        static EventDispatcher<KeyReleasedEvent> onKeyReleased;

    private:
        friend class Window;
    };

}
