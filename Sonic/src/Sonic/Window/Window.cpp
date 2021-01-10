#include <iostream>
#include <GLFW/glfw3.h>
#include "Window.h"

struct WindowData
{
    GLFWwindow* glfwID;
    float width;
    float height;
    const char* title;
};

static WindowData s_Data;

namespace Sonic {

    EventDispatcher<KeyPressedEvent>* Window::onKeyPressed;
    EventDispatcher<KeyReleasedEvent>* Window::onKeyReleased;

    EventDispatcher<MouseMovedEvent>* Window::onMouseMoved;
    EventDispatcher<MouseDraggedEvent>* Window::onMouseDragged;
    EventDispatcher<MouseButtonPressedEvent>* Window::onMouseButtonPressed;
    EventDispatcher<MouseButtonReleasedEvent>* Window::onMouseButtonReleased;
    EventDispatcher<MouseScrolledEvent>* Window::onMouseScrolled;

    EventDispatcher<WindowClosedEvent>* Window::onWindowClosed;
    EventDispatcher<WindowResizedEvent>* Window::onWindowResized;

    bool Window::init(int width, int height, const char* title)
    {
        Keyboard::init();
        Mouse::init();

        Window::onKeyPressed = &Keyboard::onKeyPressed;
        Window::onKeyReleased = &Keyboard::onKeyReleased;
        
        Window::onMouseMoved = &Mouse::onMouseMoved;
        Window::onMouseDragged = &Mouse::onMouseDragged;
        Window::onMouseButtonPressed = &Mouse::onMouseButtonPressed;
        Window::onMouseButtonReleased = &Mouse::onMouseButtonReleased;
        Window::onMouseScrolled = &Mouse::onMouseScrolled;

        Window::onWindowClosed = new EventDispatcher<WindowClosedEvent>();
        Window::onWindowResized = new EventDispatcher<WindowResizedEvent>();

        if (!glfwInit())
            return false;

        glfwSetErrorCallback(
            [](int errorCode, const char* description)
            {
                std::cout << "GLFW Error: [" << errorCode << "] " <<  description << std::endl;
            }
        );
        
        s_Data.width = static_cast<float>(width);
        s_Data.height = static_cast<float>(height);
        s_Data.title = title;

        s_Data.glfwID = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!s_Data.glfwID)
        {
            destroy();
            return false;
        }

        glfwMakeContextCurrent(s_Data.glfwID);

        glClearColor(0.5f, 0.0f, 0.5f, 1.0f);

        glfwSetKeyCallback(s_Data.glfwID, Keyboard::keyCallback);
        glfwSetCursorPosCallback(s_Data.glfwID, Mouse::cursorPosCallback);
        glfwSetMouseButtonCallback(s_Data.glfwID, Mouse::buttonCallback);
        glfwSetScrollCallback(s_Data.glfwID, Mouse::scrollCallback);
        glfwSetWindowCloseCallback(s_Data.glfwID, Window::windowCloseCallback);
        glfwSetWindowSizeCallback(s_Data.glfwID, Window::windowResizeCallback);

        return true;
    }

    void Window::destroy()
    {
        glfwTerminate();
        glfwSetErrorCallback(NULL);

        delete onWindowClosed;
        delete onWindowResized;
    }

    bool Window::isClosed()
    {
        return glfwWindowShouldClose(s_Data.glfwID);
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(s_Data.glfwID);
    }

    void Window::pollEvents()
    {
        glfwPollEvents();
    }

    void Window::windowCloseCallback(GLFWwindow* window)
    {
        onWindowClosed->Dispatch(WindowClosedEvent());
    }

    void Window::windowResizeCallback(GLFWwindow* window, int width, int height)
    {
        s_Data.width = static_cast<float>(width);
        s_Data.height = static_cast<float>(height);

        onWindowResized->Dispatch(WindowResizedEvent(width, height));
    }

    float Window::getWidth()
    {
        return s_Data.width;
    }

    float Window::getHeight()
    {
        return s_Data.height;
    }

    const char* Window::getTitle()
    {
        return s_Data.title;
    }

    void Window::setTitle(const char* title)
    {
        s_Data.title = title;

        glfwSetWindowTitle(s_Data.glfwID, s_Data.title);
    }

    double Window::getTime()
    {
        return glfwGetTime();
    }
}
