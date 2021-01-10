#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Camera2D.h"
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
        /**
        * Draws a colored rectangle to the screen.
        * 
        * @param position The position of the rectangle. The z coordinate is the z-Index of the rect.
        * @param size The size of the rectangle
        * @param color The color of the rectangle
        */
        static void drawRect(const glm::vec3& position, const glm::vec2& size, const Color& color);

        /**
        * Draws a colored rectangle to the screen.
        * 
        * @param position The position of the rectangle. The z-Index is set to 0
        * @param size The size of the rectangle
        * @param color The color of the rectangle
        */
        static void drawRect(const glm::vec2& position, const glm::vec2& size, const Color& color)
        {
            drawRect({ position.x, position.y, 0.0f }, size, color);
        }

        /**
        * Draws a rectangle with a colored sprite to the screen.
        * 
        * @param position The position of the rectangle. The z coordinate is the z-Index of the rect.
        * @param size The size of the rectangle
        * @param sprite The sprite of the rectangle
        * @param color The color of the rectangle
        */
        static void drawRect(const glm::vec3& position, const glm::vec2& size, const Sprite& sprite, const Color& color);

        /**
        * Draws a rectangle with a colored sprite to the screen.
        *
        * @param position The position of the rectangle. The z-Index is set to 0
        * @param size The size of the rectangle
        * @param sprite The sprite of the rectangle
        * @param color The color of the rectangle
        */
        static void drawRect(const glm::vec2& position, const glm::vec2& size, const Sprite& sprite, const Color& color)
        {
            drawRect({ position.x, position.y, 0.0f }, size, sprite, color);
        }

        /**
        * Draws a rectangle with a sprite to the screen.
        *
        * @param position The position of the rectangle. The z coordinate is the z-Index of the rect.
        * @param size The size of the rectangle
        * @param sprite The sprite of the rectangle
        */
        static void drawRect(const glm::vec3& position, const glm::vec2& size, const Sprite& sprite)
        {
            drawRect(position, size, sprite, WHITE);
        }

        /**
        * Draws a rectangle with a sprite to the screen.
        *
        * @param position The position of the rectangle. The z-Index is set to 0
        * @param size The size of the rectangle
        * @param sprite The sprite of the rectangle
        */
        static void drawRect(const glm::vec2& position, const glm::vec2& size, const Sprite& sprite)
        {
            drawRect({ position.x, position.y, 0.0f }, size, sprite, WHITE);
        }

        /**
        * Draws a rotated colored rectangle to the screen.
        *
        * @param position The position of the rectangle. The z coordinate is the z-Index of the rect.
        * @param size The size of the rectangle
        * @param rotation The rotation of the rectangle in degrees
        * @param color The color of the rectangle
        */
        static void drawRotatedRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Color& color);

        /**
        * Draws a rotated colored rectangle to the screen.
        *
        * @param position The position of the rectangle. The z-Index is set to 0
        * @param size The size of the rectangle
        * @param rotation The rotation of the rectangle in degrees
        * @param color The color of the rectangle
        */
        static void drawRotatedRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Color& color)
        {
            drawRotatedRect({ position.x, position.y, 0.0f }, size, rotation, color);
        }

        /**
        * Draws a rotated rectangle with a colored sprite to the screen.
        *
        * @param position The position of the rectangle. The z coordinate is the z-Index of the rect.
        * @param size The size of the rectangle
        * @param rotation The rotation of the rectangle in degrees
        * @param sprite The sprite of the rectangle
        * @param color The color of the rectangle
        */
        static void drawRotatedRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color);

        /**
        * Draws a rotated rectangle with a colored sprite to the screen
        *
        * @param position The position of the rectangle. The z-Index is set to 0
        * @param size The size of the rectangle
        * @param rotation The rotation of the rectangle in degrees
        * @param sprite The sprite of the rectangle
        * @param color The color of the rectangle
        */
        static void drawRotatedRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color)
        {
            drawRotatedRect({ position.x, position.y, 0.0f }, size, rotation, sprite, color);
        }

        /**
        * Draws a rotated rectangle with a sprite to the screen
        *
        * @param position The position of the rectangle. The z coordinate is the z-Index of the rect.
        * @param size The size of the rectangle
        * @param rotation The rotation of the rectangle in degrees
        * @param sprite The sprite of the rectangle
        */
        static void drawRotatedRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite)
        {
            drawRotatedRect(position, size, rotation, sprite, WHITE);
        }

        /**
        * Draws a rotated rectangle with a sprite to the screen
        *
        * @param position The position of the rectangle. The z-Index is set to 0
        * @param size The size of the rectangle
        * @param rotation The rotation of the rectangle in degrees
        * @param sprite The sprite of the rectangle
        */
        static void drawRotatedRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Sprite& sprite)
        {
            drawRotatedRect({ position.x, position.y, 0.0f }, size, rotation, sprite, WHITE);
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
