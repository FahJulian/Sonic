#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Sonic/Event/Events.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Keys.h"

namespace Sonic {

    class Keyboard
    {
    private:
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    public:
        static bool isKeyPressed(Key key);

    private:
        friend class Window;
    };

}
