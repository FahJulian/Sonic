#include "Sonic/App.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/Scene.h"
#include "Mouse.h"

using namespace Sonic;


struct MouseData
{
    float x = 0.0f;
    float y = 0.0f;
    bool pressedButtons[GLFW_MOUSE_BUTTON_LAST] = { false };
};

static MouseData s_Data = MouseData();


void Mouse::cursorPosCallback(GLFWwindow* window, double posX, double posY)
{
    int _, height;
    glfwGetWindowSize(window, &_, &height);

    float x = static_cast<float>(posX);
    float y = static_cast<float>(height) - static_cast<float>(posY);

    for (int button = 0; button < GLFW_MOUSE_BUTTON_LAST; button++)
        if (s_Data.pressedButtons[button])
            App::getActiveScene()->DispatchEvent(MouseDraggedEvent(button, x, y, s_Data.x, s_Data.y, x - s_Data.x, y - s_Data.y));

    App::getActiveScene()->DispatchEvent(MouseMovedEvent(x, y, s_Data.x, s_Data.y, x - s_Data.x, y - s_Data.y));

    s_Data.x = x;
    s_Data.y = y;
}

void Mouse::buttonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        s_Data.pressedButtons[button] = true;
        App::getActiveScene()->DispatchEvent(MouseButtonPressedEvent(button, s_Data.x, s_Data.y));
    }
    else if (action == GLFW_RELEASE)
    {
        s_Data.pressedButtons[button] = false;
        App::getActiveScene()->DispatchEvent(MouseButtonReleasedEvent(button, s_Data.x, s_Data.y));
    }
}

void Mouse::scrollCallback(GLFWwindow* window, double scrollX, double scrollY)
{
    App::getActiveScene()->DispatchEvent(MouseScrolledEvent(s_Data.x, s_Data.y, static_cast<float>(scrollX), static_cast<float>(scrollY)));
}

bool Mouse::isButtonPressed(MouseButton button) 
{ 
    return s_Data.pressedButtons[button]; 
}
    
float Mouse::getX() 
{ 
    return s_Data.x; 
}

float Mouse::getY() 
{ 
    return s_Data.y; 
}
