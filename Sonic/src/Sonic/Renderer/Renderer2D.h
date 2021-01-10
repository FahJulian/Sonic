#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Camera2D.h"
#include "Sprite.h"
#include "Color.h"

static const Sonic::Color WHITE = Sonic::Color(0xffffffff);

namespace Sonic {

    class Renderer2D
    {
    public:
        static void drawRect(const glm::vec3& position, const glm::vec2& size, const Color& color);

        static void drawRect(const glm::vec2& position, const glm::vec2& size, const Color& color)
        {
            drawRect({ position.x, position.y, 0.0f }, size, color);
        }

        static void drawRect(const glm::vec3& position, const glm::vec2& size, const Sprite& sprite, const Color& color);

        static void drawRect(const glm::vec2& position, const glm::vec2& size, const Sprite& sprite, const Color& color)
        {
            drawRect({ position.x, position.y, 0.0f }, size, sprite, color);
        }

        static void drawRect(const glm::vec3& position, const glm::vec2& size, const Sprite& sprite)
        {
            drawRect(position, size, sprite, WHITE);
        }

        static void drawRect(const glm::vec2& position, const glm::vec2& size, const Sprite& sprite)
        {
            drawRect({ position.x, position.y, 0.0f }, size, sprite, WHITE);
        }

        static void drawRotatedRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Color& color);

        static void drawRotatedRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Color& color)
        {
            drawRotatedRect({ position.x, position.y, 0.0f }, size, rotation, color);
        }

        static void drawRotatedRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color);

        static void drawRotatedRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color)
        {
            drawRotatedRect({ position.x, position.y, 0.0f }, size, rotation, sprite, color);
        }

        static void drawRotatedRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite)
        {
            drawRotatedRect(position, size, rotation, sprite, WHITE);
        }

        static void drawRotatedRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Sprite& sprite)
        {
            drawRotatedRect({ position.x, position.y, 0.0f }, size, rotation, sprite, WHITE);
        }

        static void init();
        
        static void setClearColor(const Color& color) { glClearColor(color.r, color.g, color.b, color.a); }

        static void startScene(const Camera2D* camera);
        static void endScene();
    };

}
