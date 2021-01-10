#include "Sonic/App.h"
#include "Keyboard.h"

struct KeyboardData
{
    bool m_PressedKeys[GLFW_KEY_LAST] = { false };
};

static KeyboardData s_Data = KeyboardData();


namespace Sonic {

    void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            s_Data.m_PressedKeys[key] = true;
            App::get()->GetActiveScene()->DispatchEvent(KeyPressedEvent(key));
        } 
        else if (action == GLFW_RELEASE)
        {
            s_Data.m_PressedKeys[key] = false;
            App::get()->GetActiveScene()->DispatchEvent(KeyReleasedEvent(key));
        }
    }

    bool Keyboard::isKeyPressed(Key key) 
    { 
        return s_Data.m_PressedKeys[key]; 
    }

}
