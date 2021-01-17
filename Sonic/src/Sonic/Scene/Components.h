#pragma once
#include <glm/glm.hpp>
#include "../Renderer/Sprite.h"
#include "../Renderer/Color.h"

namespace Sonic {

    struct Direction2DComponent
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

        Direction2DComponent(Direction direction)
            : direction(direction), lastDirection(direction) {}
        Direction2DComponent(Direction direction, Direction lastDirection)
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

    struct Renderer2DComponent
    {
        Sprite sprite;
        Color color;

        Renderer2DComponent(const Sprite& sprite, const Color& color)
            : sprite(sprite), color(color) {}
        Renderer2DComponent(const Sprite& sprite)
            : sprite(sprite), color(Colors::White) {}
        Renderer2DComponent(const Color& color)
            : sprite(Sprite()), color(color) {}
    };

    struct Camera2DComponent
    {
        Camera2D camera;
        bool isSceneCamera;

        Camera2DComponent(const Camera2D& camera, bool isSceneCamera = false)
            : camera(camera), isSceneCamera(isSceneCamera) {}
    };

}
