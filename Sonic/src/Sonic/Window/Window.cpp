#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Sonic/Debug/Log/Log.h"
#include "Sonic/Graphics/Color.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/Scene.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Sonic/App.h"
#include "Window.h"

using namespace Sonic;


struct WindowData
{
    GLFWwindow* glfwID;
    float width;
    float height;
    std::string title;
};

static WindowData s_Data;


bool Window::init(int width, int height, const std::string& title, bool resizable)
{
    if (!glfwInit())
        return false;

    glfwSetErrorCallback(
        [](int errorCode, const char* description)
        {
            SONIC_LOG_ERROR("GLFW Error [", errorCode, "]: ", description);
        }
    );

    StandardCursors::init();
        
    s_Data.width = static_cast<float>(width);
    s_Data.height = static_cast<float>(height);
    s_Data.title = title;

    glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

    s_Data.glfwID = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
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
    WindowClosedEvent e;
    App::getActiveScene()->DispatchEvent(e);
    App::onWindowClosed(e);
}

void Window::windowResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    s_Data.width = static_cast<float>(width);
    s_Data.height = static_cast<float>(height);

    WindowResizedEvent e = WindowResizedEvent(s_Data.width, s_Data.height);
    App::getActiveScene()->DispatchEvent(e);
    App::onWindowResized(e);
}

float Window::getWidth()
{
    return s_Data.width;
}

float Window::getHeight()
{
    return s_Data.height;
}

void Window::setWidth(float width)
{
    glfwSetWindowSize(s_Data.glfwID, static_cast<int>(width), static_cast<int>(getHeight()));
}

void Window::setHeight(float height)
{
    glfwSetWindowSize(s_Data.glfwID, static_cast<int>(getWidth()), static_cast<int>(height));
}

void Window::setSize(float width, float height)
{
    glfwSetWindowSize(s_Data.glfwID, static_cast<int>(width), static_cast<int>(height));
}

const std::string& Window::getTitle()
{
    return s_Data.title;
}

void Window::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
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
