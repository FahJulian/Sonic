#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "../Event/Events.h"
#include "../Event/EventDispatcher.h"
#include "../Renderer/Color.h"

namespace Sonic {

    class Window
    {
    public:
        static EventDispatcher<KeyPressedEvent>* onKeyPressed;
        static EventDispatcher<KeyReleasedEvent>* onKeyReleased;

        static EventDispatcher<MouseMovedEvent>* onMouseMoved;
        static EventDispatcher<MouseDraggedEvent>* onMouseDragged;
        static EventDispatcher<MouseButtonPressedEvent>* onMouseButtonPressed;
        static EventDispatcher<MouseButtonReleasedEvent>* onMouseButtonReleased;
        static EventDispatcher<MouseScrolledEvent>* onMouseScrolled;

        static EventDispatcher<WindowClosedEvent>* onWindowClosed;
        static EventDispatcher<WindowResizedEvent>* onWindowResized;

        static bool init(int width, int height, const char* title);
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
