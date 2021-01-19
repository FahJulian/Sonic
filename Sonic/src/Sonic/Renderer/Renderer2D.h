#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Sonic/Graphics/Graphics2D/Texture.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"
#include "Sonic/Graphics/Color.h"
#include "Camera2D.h"

static const Sonic::Color WHITE = Sonic::Color(0xffffffff);

namespace Sonic {

    namespace Renderer2D {

        void drawRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color);

        void drawRect(const glm::vec3& position, const glm::vec2& size, const Sprite& sprite, const Color& color);

        void drawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color);

        void drawRect(const glm::vec2& position, const glm::vec2& size, const Sprite& sprite, const Color& color);

        void drawRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Color& color);

        void drawRect(const glm::vec3& position, const glm::vec2& size, const Color& color);

        void drawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Color& color);

        void drawRect(const glm::vec2& position, const glm::vec2& size, const Color& color);

        void drawRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite);

        void drawRect(const glm::vec3& position, const glm::vec2& size, const Sprite& sprite);

        void drawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Sprite& sprite);

        void drawRect(const glm::vec2& position, const glm::vec2& size, const Sprite& sprite);

        void init();
        
        void setClearColor(const Color& color);

        void startScene(const Camera2D* camera);

        void endScene();
    };

}
