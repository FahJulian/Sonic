#pragma once
#include <glm/glm.hpp>
#include "../Renderer/Sprite.h"
#include "../Renderer/Color.h"

namespace Sonic {

    struct DirectionComponent
    {
        enum class Direction : char
        {
            Up = 2,
            Down = -2,
            Right = 1,
            Left = -1
        };

        Direction direction;
        Direction lastDirection;

        DirectionComponent(Direction direction)
            : direction(direction), lastDirection(direction) {}
        DirectionComponent(Direction direction, Direction lastDirection)
            : direction(direction), lastDirection(lastDirection) {}
    };

    struct Transform2DComponent
    {   
        glm::vec3 position;
        glm::vec2 scale;
        float rotation;

        Transform2DComponent()
            : position({ 0, 0, 0 }), scale({ 0, 0 }), rotation(0) {}
        Transform2DComponent(const glm::vec3& position, const glm::vec2& scale, const float rotation)
            : position(position), scale(scale), rotation(rotation) {}
        Transform2DComponent(const glm::vec2& position, const glm::vec2& scale, const float rotation)
            : position({ position.x, position.y, 0.0f }), scale(scale), rotation(rotation) {}
        Transform2DComponent(float x, float y, float z, float width, float height, float rotation)
            : position({ x, y, z }), scale({ width, height }), rotation(rotation) {}
        Transform2DComponent(float x, float y, float z, float width, float height)
            : position({ x, y, z }), scale(width, height), rotation(0.0f) {}
        Transform2DComponent(float x, float y, float width, float height)
            : position({ x, y, 0.0f }), scale({ width, height }), rotation(0.0f) {}
    };

    struct SpriteComponent
    {
        Sprite sprite;

        SpriteComponent(const Sprite& sprite)
            : sprite(sprite) {}
        SpriteComponent(const SpriteComponent& other)
            : sprite(other.sprite) {}

        SpriteComponent operator=(const SpriteComponent& other)
        {
            return SpriteComponent(other);
        }
    };

    struct ColorComponent
    {
        Color color;

        ColorComponent(const Color& color)
            : color(color) {}
        ColorComponent(float r, float g, float b, float a)
            : color({ r, g, b, a }) {}
    };

    struct ColoredSpriteComponent
    {
        Color color;
        Sprite sprite;

        ColoredSpriteComponent(const Color& color, const Sprite& sprite)
            : color(color), sprite(sprite) {}
        ColoredSpriteComponent(const ColoredSpriteComponent& other)
            : color(other.color), sprite(other.sprite) {}

        ColoredSpriteComponent operator=(const ColoredSpriteComponent& other)
        {
            return ColoredSpriteComponent(other);
        }
    };

    struct Camera2DComponent
    {
        Camera2D camera;
        bool isSceneCamera;

        Camera2DComponent(const Camera2D& camera, bool isSceneCamera = false)
            : camera(camera), isSceneCamera(isSceneCamera) {}
    };

}
