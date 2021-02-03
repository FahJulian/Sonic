#include "Sonic/App.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Event/Events.h"
#include "Keyboard.h"

using namespace Sonic;


struct KeyboardData
{
    bool m_PressedKeys[GLFW_KEY_LAST] = { false };
};

static KeyboardData s_Data = KeyboardData();


void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        s_Data.m_PressedKeys[key] = true;
        App::getActiveScene()->DispatchEvent(KeyPressedEvent(key));
    } 
    else if (action == GLFW_RELEASE)
    {
        s_Data.m_PressedKeys[key] = false;
        App::getActiveScene()->DispatchEvent(KeyReleasedEvent(key));
    }
}

bool Keyboard::isKeyPressed(Key key) 
{ 
    return s_Data.m_PressedKeys[key]; 
}
