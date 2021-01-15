#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>

namespace Sonic {

    using MouseButton = unsigned char;
    
    namespace Buttons {
        
        enum : MouseButton
        {
            Left = GLFW_MOUSE_BUTTON_LEFT,
            Right = GLFW_MOUSE_BUTTON_RIGHT,
            Middle = GLFW_MOUSE_BUTTON_MIDDLE,
            MB_4 = GLFW_MOUSE_BUTTON_4,
            MB_5 = GLFW_MOUSE_BUTTON_5,
            MB_6 = GLFW_MOUSE_BUTTON_6,
            MB_7 = GLFW_MOUSE_BUTTON_7,
            MB_8 = GLFW_MOUSE_BUTTON_8
        };

    }

}
