#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Camera2D.h"
#include "Texture.h"
#include "Sprite.h"
#include "Color.h"

static const Sonic::Color WHITE = Sonic::Color(0xffffffff);

namespace Sonic {

    /**
    * Handles drawing of simple 2D objects to the screen. For now only rectangles can be drawn
    */
    class Renderer2D
    {
    private:
        Renderer2D() = delete;
        Renderer2D(const Renderer2D& other) = delete;
        ~Renderer2D() = delete;

    public:
        static void drawRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color);

        static void drawRect(const glm::vec3& position, const glm::vec2& size, const Sprite& sprite, const Color& color)
        {
            drawRect(position, size, 0.0f, sprite, color);
        }

        static void drawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, rotation, sprite, color);
        }

        static void drawRect(const glm::vec2& position, const glm::vec2& size, const Sprite& sprite, const Color& color)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, 0.0f, sprite, color);
        }

        static void drawRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Color& color)
        {
            drawRect(position, size, rotation, Sprite(), color);
        }

        static void drawRect(const glm::vec3& position, const glm::vec2& size, const Color& color)
        {
            drawRect(position, size, 0.0f, Sprite(), color);
        }

        static void drawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Color& color)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, rotation, Sprite(), color);
        }

        static void drawRect(const glm::vec2& position, const glm::vec2& size, const Color& color)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, 0.0f, Sprite(), color);
        }

        static void drawRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite)
        {
            drawRect(position, size, rotation, sprite, WHITE);
        }

        static void drawRect(const glm::vec3& position, const glm::vec2& size, const Sprite& sprite)
        {
            drawRect(position, size, 0.0f, sprite, WHITE);
        }

        static void drawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Sprite& sprite)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, rotation, sprite, WHITE);
        }

        static void drawRect(const glm::vec2& position, const glm::vec2& size, const Sprite& sprite)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, 0.0f, sprite, WHITE);
        }

        /**
        * Initializes the 2D Renderer
        */
        static void init();
        
        /*
        * Set the clear color of the screen
        */
        static void setClearColor(const Color& color) { glClearColor(color.r, color.g, color.b, color.a); }

        /**
        * Start a new scene. If there are Objects on the screen that havent been flushed
        * with Renderer2D::endScene(), they are deleted.
        */
        static void startScene(const Camera2D* camera);

        /**
        * Flushes the Objects on the screen.
        */
        static void endScene();
    };

}
