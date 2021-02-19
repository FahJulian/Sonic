#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <glfw/glfw3native.h>
#include <uxtheme.h>
#include <WinUser.h>
#include "Sonic/Renderer/Font/FontRenderer.h"
#include "Sonic/Renderer/UI/UIRenderer.h"
#include "Sonic/Scene/SceneManager.h"
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
    GLFWwindow* glfwID = nullptr;
    HWND nativeHandle = 0;
    GLFWmonitor* monitor = nullptr;
    WindowMode mode;
    int unmaximizedWidth = 0;
    int unmaximizedHeight = 0;
    int currentWidth = 0;
    int currentHeight = 0;
    bool decorated = false;
    String title;
};

static WindowData s_Data;
static bool s_IgnoreGlfwResizeCallback = false;


bool Window::init(WindowMode mode, const Color& clearColor, const String& title, int width, int height, bool resizable)
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
        
    s_Data.title = title;
    s_Data.unmaximizedWidth = width;
    s_Data.unmaximizedHeight = height;
    s_Data.mode = mode;
    s_Data.decorated = mode == WindowMode::Windowed || mode == WindowMode::Fullscreen;
    
    s_Data.monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidmode = glfwGetVideoMode(s_Data.monitor);

    glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
    glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
    glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
    glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
    glfwWindowHint(GLFW_RESIZABLE, resizable);
    glfwWindowHint(GLFW_DECORATED, s_Data.decorated);
    glfwWindowHint(GLFW_VISIBLE, false);

    if (mode == WindowMode::Windowed || mode == WindowMode::Borderless)
    {
        s_Data.currentWidth = s_Data.unmaximizedWidth;
        s_Data.currentHeight = s_Data.unmaximizedHeight;
    }
    else
    {
        s_Data.currentWidth = vidmode->width;
        s_Data.currentHeight = vidmode->height;
    }

    s_Data.glfwID = glfwCreateWindow(42, 42, s_Data.title.c_str(), NULL, NULL);
    s_Data.nativeHandle = glfwGetWin32Window(s_Data.glfwID);

    if (!s_Data.glfwID || !s_Data.nativeHandle)
    {
        destroy();
        return false;
    }

    s_IgnoreGlfwResizeCallback = true;

    if (mode == WindowMode::BorderlessFullscreen)
    {
        SetWindowLong(s_Data.nativeHandle, GWL_STYLE, 0);
        SetWindowLong(s_Data.nativeHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);
        SetWindowPos(s_Data.nativeHandle, 0, 0, 0, vidmode->width, vidmode->height, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
    }
    else if (mode == WindowMode::Borderless)
    {
        SetWindowLong(s_Data.nativeHandle, GWL_STYLE, 0);
        SetWindowLong(s_Data.nativeHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);
        SetWindowPos(s_Data.nativeHandle, 0, (vidmode->width - s_Data.unmaximizedWidth) / 2, (vidmode->height - s_Data.unmaximizedHeight) / 2, s_Data.unmaximizedWidth, s_Data.unmaximizedHeight, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
    }
    else if (mode == WindowMode::Windowed)
    {
        SetWindowLong(s_Data.nativeHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowLong(s_Data.nativeHandle, GWL_EXSTYLE, 0);
        SetWindowPos(s_Data.nativeHandle, 0, (vidmode->width - s_Data.unmaximizedWidth) / 2, (vidmode->height - s_Data.unmaximizedHeight) / 2, s_Data.unmaximizedWidth, s_Data.unmaximizedHeight, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
        glfwSetWindowMonitor(s_Data.glfwID, NULL, (vidmode->width - s_Data.unmaximizedWidth) / 2, (vidmode->height - s_Data.unmaximizedHeight) / 2, s_Data.unmaximizedWidth, s_Data.unmaximizedHeight, vidmode->refreshRate);
    }
    else if (mode == WindowMode::Fullscreen)
    {
        glfwSetWindowMonitor(s_Data.glfwID, s_Data.monitor, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
    }

    s_IgnoreGlfwResizeCallback = false;

    setCursor(StandardCursors::Arrow);

    glfwShowWindow(s_Data.glfwID);

    glfwMakeContextCurrent(s_Data.glfwID);
    setClearColor(clearColor);

    glfwSetKeyCallback(s_Data.glfwID, Keyboard::keyCallback);
    glfwSetCursorPosCallback(s_Data.glfwID, Mouse::cursorPosCallback);
    glfwSetMouseButtonCallback(s_Data.glfwID, Mouse::buttonCallback);
    glfwSetScrollCallback(s_Data.glfwID, Mouse::scrollCallback);
    glfwSetWindowCloseCallback(s_Data.glfwID, Window::windowCloseCallback);
    glfwSetWindowSizeCallback(s_Data.glfwID, Window::glfwWindowResizeCallback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glViewport(0, 0, s_Data.currentWidth, s_Data.currentHeight);

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
    EventDispatcher::dispatch(e);
    App::onWindowClosed(e);
}

void Window::glfwWindowResizeCallback(GLFWwindow* window, int width, int height)
{
    if (s_IgnoreGlfwResizeCallback)
        return;

    windowResizeCallback(s_Data.unmaximizedWidth = width, s_Data.unmaximizedHeight = height);
}

void Window::windowResizeCallback(int width, int height)
{
    glViewport(0, 0, width, height);

    WindowResizedEvent e = WindowResizedEvent(static_cast<float>(s_Data.currentWidth = width), static_cast<float>(s_Data.currentHeight = height));
    EventDispatcher::dispatch(e);
    App::onWindowResized(e);
}

float Window::getWidth()
{
    return static_cast<float>(s_Data.currentWidth);
}

float Window::getHeight()
{
    return static_cast<float>(s_Data.currentHeight);
}

void Window::setWidth(float width)
{
    s_Data.unmaximizedWidth = static_cast<int>(width);

    if (s_Data.mode == WindowMode::Windowed || s_Data.mode == WindowMode::Borderless)
        glfwSetWindowSize(s_Data.glfwID, s_Data.unmaximizedWidth, s_Data.unmaximizedHeight);
}

void Window::setHeight(float height)
{
    s_Data.unmaximizedHeight = static_cast<int>(height);

    if (s_Data.mode == WindowMode::Windowed || s_Data.mode == WindowMode::Borderless)
        glfwSetWindowSize(s_Data.glfwID, s_Data.unmaximizedWidth, s_Data.unmaximizedHeight);
}

void Window::setSize(float width, float height)
{
    s_Data.unmaximizedWidth = static_cast<int>(width);
    s_Data.unmaximizedHeight = static_cast<int>(height);

    if (s_Data.mode == WindowMode::Windowed)
    {
        glfwSetWindowSize(s_Data.glfwID, s_Data.unmaximizedWidth, s_Data.unmaximizedHeight);
    }
    else if (s_Data.mode == WindowMode::Borderless)
    {
        const GLFWvidmode* vidmode = glfwGetVideoMode(s_Data.monitor);
        SetWindowPos(s_Data.nativeHandle, 0, (vidmode->width - s_Data.unmaximizedWidth) / 2, (vidmode->height - s_Data.unmaximizedHeight) / 2, s_Data.unmaximizedWidth, s_Data.unmaximizedHeight, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
    }
}

void Window::setWindowMode(WindowMode newMode)
{
    s_IgnoreGlfwResizeCallback = true;

    s_Data.mode = newMode;

    const GLFWvidmode* vidmode = glfwGetVideoMode(s_Data.monitor);

    if (newMode == WindowMode::BorderlessFullscreen)
    {
        SetWindowLong(s_Data.nativeHandle, GWL_STYLE, 0);
        SetWindowLong(s_Data.nativeHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);
        SetWindowPos(s_Data.nativeHandle, 0, 0, 0, vidmode->width, vidmode->height, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
        windowResizeCallback(vidmode->width, vidmode->height);
    }
    else if (newMode == WindowMode::Borderless)
    {
        SetWindowLong(s_Data.nativeHandle, GWL_STYLE, 0);
        SetWindowLong(s_Data.nativeHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);
        SetWindowPos(s_Data.nativeHandle, 0, (vidmode->width - s_Data.unmaximizedWidth) / 2, (vidmode->height - s_Data.unmaximizedHeight) / 2, s_Data.unmaximizedWidth, s_Data.unmaximizedHeight, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
        windowResizeCallback(s_Data.unmaximizedWidth, s_Data.unmaximizedHeight);
    }
    else if (newMode == WindowMode::Windowed)
    {
        SetWindowLong(s_Data.nativeHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowLong(s_Data.nativeHandle, GWL_EXSTYLE, 0);
        SetWindowPos(s_Data.nativeHandle, 0, (vidmode->width - s_Data.unmaximizedWidth) / 2, (vidmode->height - s_Data.unmaximizedHeight) / 2, s_Data.unmaximizedWidth, s_Data.unmaximizedHeight, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
        glfwSetWindowMonitor(s_Data.glfwID, NULL, (vidmode->width - s_Data.unmaximizedWidth) / 2, (vidmode->height - s_Data.unmaximizedHeight) / 2, s_Data.unmaximizedWidth, s_Data.unmaximizedHeight, vidmode->refreshRate);
        windowResizeCallback(s_Data.unmaximizedWidth, s_Data.unmaximizedHeight);
    }
    else if (newMode == WindowMode::Fullscreen)
    {
        glfwSetWindowMonitor(s_Data.glfwID, s_Data.monitor, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
        windowResizeCallback(vidmode->width, vidmode->height);
    }

    s_IgnoreGlfwResizeCallback = false;
}

WindowMode Window::getWindowMode()
{
    return s_Data.mode;
}

const String& Window::getTitle()
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

void Window::setTitle(const String& title)
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
