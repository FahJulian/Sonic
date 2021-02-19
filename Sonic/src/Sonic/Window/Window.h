#pragma once
#include <string>
#include "Sonic/Base.h"
#include "Cursor/Cursors.h"

namespace Sonic {

    enum class WindowMode : uint8_t
    {
        Windowed,
        Borderless,
        BorderlessFullscreen,
        Fullscreen
    };

    struct Color;

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
        static bool init(WindowMode mode, const Color& clearColor, const String& title, int width, int height, bool resizable = true);

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

        static void setClearColor(const Color& color);

        static void clear();

        /**
        * @return The time that has since window initialization passed in seconds
        */
        static double getTime();

        static float getHeight();

        static float getWidth();

        static void setSize(float width, float height);

        static void setWidth(float width);

        static void setHeight(float height);

        static void setWindowMode(WindowMode newMode);

        static WindowMode getWindowMode();

        static const String& getTitle();

        static void setTitle(const String& title);

        static void setCursor(Cursor cursor);

        static void setCursor(StandardCursor cursor);

        static void setDefaultCursorSet(StandardCursorSet cursorSet);

    private:
        static void windowCloseCallback(GLFWwindow* window);

        static void glfwWindowResizeCallback(GLFWwindow* window, int width, int height);

        static void windowResizeCallback(int width, int height);
    };
    
}
