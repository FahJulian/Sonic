#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Graphics/Color.h"

namespace Sonic {

    /**
    * Uninstantiable class that is responsible for initializing glfw windows, turning glfw 
    * window events into Sonic events and calling the scenes event dispatcher with that event
    * and wrapping around other glfw functions
    */
    class Window
    {
    public:
        /**
        * Initializes the Window class by creating a glfw window with the given props
        * 
        * @param width The width of the created window
        * @param height The height of the created window
        * @param title The title of the created window
        * @param resizable Whether or not the created window should be resizable by the user
        * 
        * @return Whether the initialization was successful
        */
        static bool init(int width, int height, const char* title, bool resizable = true);

        /**
        * Destroys the glfw window and frees all memory related to glfw
        */
        static void destroy();

        /**
        * @return Whether or not the window has been closed
        */
        static bool isClosed();

        /**
        * Swaps the buffers of the glfw window
        */
        static void swapBuffers();

        /**
        * Polls all glfw window events
        */
        static void pollEvents();

        /**
        * @return The time that has since window initialization passed in seconds
        */
        static double getTime();

        static float getHeight();

        static float getWidth();

        static const std::string& getTitle();

        static void setTitle(const std::string& title);

    private:
        static void windowCloseCallback(GLFWwindow* window);
        static void windowResizeCallback(GLFWwindow* window, int width, int height);
    };
    
}
