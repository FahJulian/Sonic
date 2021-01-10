#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Renderer/Color.h"

namespace Sonic {

    class Window
    {
    public:
        static bool init(int width, int height, const char* title, bool resizable = true);
        static void destroy();

        static bool isClosed();
        static void swapBuffers();
        static void pollEvents();
        static double getTime();

        static float getHeight();
        static float getWidth();
        static const char* getTitle();

        void setTitle(const char* title);

    private:
        static void windowCloseCallback(GLFWwindow* window);
        static void windowResizeCallback(GLFWwindow* window, int width, int height);
    };
    
}
