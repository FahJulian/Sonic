#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Sonic/Graphics/Texture.h"
#include "Sonic/Graphics/Sprite.h"
#include "Sonic/Graphics/Color.h"
#include "Sonic/Scene/Components/2DComponents.h"
#include "Camera2D.h"

static const Sonic::Color WHITE = Sonic::Color(0xffffffff);

namespace Sonic {

    class Scene;

    class Renderer2D 
    {
    public:
        static void init();
        static void rebuffer(Scene* scene, const Camera2D* camera);
        static void render();
        static void destroy();

    private:
        static void drawRect(int index, const glm::vec3& position, const glm::vec2& scale, float rotation, const Sprite& sprite, const Color& color);
        static void drawEntity(Scene* scene, Entity e, Renderer2DComponent* r, int index);
    };

}