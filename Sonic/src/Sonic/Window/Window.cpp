#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Sonic/Graphics/Color.h"
#include "Sonic/App.h"
#include "Window.h"

struct WindowData
{
    GLFWwindow* glfwID;
    float width;
    float height;
    std::string title;
};

static WindowData s_Data;

namespace Sonic {

    bool Window::init(int width, int height, const char* title, bool resizable)
    {
        if (!glfwInit())
            return false;

        glfwSetErrorCallback(
            [](int errorCode, const char* description)
            {
                std::cout << "GLFW Error: [" << errorCode << "] " <<  description << std::endl;
            }
        );

        StandardCursors::init();
        
        s_Data.width = static_cast<float>(width);
        s_Data.height = static_cast<float>(height);
        s_Data.title = title;

        glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

        s_Data.glfwID = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!s_Data.glfwID)
        {
            destroy();
            return false;
        }

        setCursor(StandardCursors::Arrow);

        glfwMakeContextCurrent(s_Data.glfwID);

        setClearColor(Colors::DarkGray);

        glfwSetKeyCallback(s_Data.glfwID, Keyboard::keyCallback);
        glfwSetCursorPosCallback(s_Data.glfwID, Mouse::cursorPosCallback);
        glfwSetMouseButtonCallback(s_Data.glfwID, Mouse::buttonCallback);
        glfwSetScrollCallback(s_Data.glfwID, Mouse::scrollCallback);
        glfwSetWindowCloseCallback(s_Data.glfwID, Window::windowCloseCallback);
        glfwSetWindowSizeCallback(s_Data.glfwID, Window::windowResizeCallback);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        return true;
    }

    void Window::destroy()
    {
        glfwTerminate();
        glfwSetErrorCallback(NULL);
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
        App::get()->GetActiveScene()->DispatchEvent(WindowClosedEvent());
    }

    void Window::windowResizeCallback(GLFWwindow* window, int width, int height)
    {
        s_Data.width = static_cast<float>(width);
        s_Data.height = static_cast<float>(height);

        App::get()->GetActiveScene()->DispatchEvent(WindowResizedEvent(width, height));
    }

    float Window::getWidth()
    {
        return s_Data.width;
    }

    float Window::getHeight()
    {
        return s_Data.height;
    }

    const std::string& Window::getTitle()
    {
        return s_Data.title;
    }

    void Window::setClearColor(const Color& newClearColor)
    {
        glClearColor(newClearColor.r, newClearColor.g, newClearColor.b, newClearColor.a);
    }

    void Window::setTitle(const std::string& title)
    {
        glfwSetWindowTitle(s_Data.glfwID, title.c_str());
        s_Data.title = title;
    }

    double Window::getTime()
    {
        return glfwGetTime();
    }

    void Window::setCursor(Cursor cursor)
    {
        glfwSetCursor(s_Data.glfwID, cursor);
    }

    void Window::setCursor(StandardCursor cursor)
    {
        glfwSetCursor(s_Data.glfwID, StandardCursors::getCurrent(cursor));
    }

    void Window::setDefaultCursorSet(StandardCursorSet cursorSet)
    {
        StandardCursors::setStandardCursorSet(cursorSet);
    }
}
