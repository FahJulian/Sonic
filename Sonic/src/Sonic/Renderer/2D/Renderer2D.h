#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Sonic/Graphics/Texture.h"
#include "Sonic/Graphics/Sprite.h"
#include "Sonic/Graphics/Color.h"
#include "Camera2D.h"

static const Sonic::Color WHITE = Sonic::Color(0xffffffff);

namespace Sonic {

    class Scene;

    namespace Renderer2D {

        void init();

        void rebuffer(Scene* scene, const Camera2D* camera);

        void render();
    };

}
