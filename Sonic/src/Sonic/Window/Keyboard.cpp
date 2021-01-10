#include "Keyboard.h"

struct KeyboardData
{
    bool m_PressedKeys[GLFW_KEY_LAST] = { false };
};

static KeyboardData s_Data;


namespace Sonic {

    EventDispatcher<KeyPressedEvent> Keyboard::onKeyPressed;
    EventDispatcher<KeyReleasedEvent> Keyboard::onKeyReleased;

    void Keyboard::init()
    {
        s_Data = KeyboardData();
        Keyboard::onKeyPressed = EventDispatcher<KeyPressedEvent>();
        Keyboard::onKeyReleased = EventDispatcher<KeyReleasedEvent>();
    }

    void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            s_Data.m_PressedKeys[key] = true;
            Keyboard::onKeyPressed.Dispatch(KeyPressedEvent(key));
        } 
        else if (action == GLFW_RELEASE)
        {
            s_Data.m_PressedKeys[key] = false;
            Keyboard::onKeyReleased.Dispatch(KeyReleasedEvent(key));
        }
    }

    bool Keyboard::isKeyPressed(Key key) 
    { 
        return s_Data.m_PressedKeys[key]; 
    }

}
