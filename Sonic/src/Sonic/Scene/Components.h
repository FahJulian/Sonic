#pragma once
#include <glm/glm.hpp>
#include "../Renderer/Sprite.h"
#include "../Renderer/Color.h"
#include "Component.h"

namespace Sonic {

    struct Transform2DComponent
    {   
        SONIC_REGISTER_COMPONENT(3)

        glm::vec3 position;
        glm::vec2 scale;
        float rotation;

        Transform2DComponent(const glm::vec3& position, const glm::vec2& scale, const float rotation)
            : position(position), scale(scale), rotation(rotation)
        {
        }

        Transform2DComponent(const glm::vec2& position, const glm::vec2& scale, const float rotation)
            : position({ position.x, position.y, 0.0f }), scale(scale), rotation(rotation)
        {
        }

        Transform2DComponent(float x, float y, float z, float width, float height, float rotation)
            : position({ x, y, z }), scale({ width, height }), rotation(rotation)
        {
        }

        Transform2DComponent(float x, float y, float z, float width, float height)
            : position({ x, y, z }), scale(width, height), rotation(0.0f)
        {
        }

        Transform2DComponent(float x, float y, float width, float height)
            : position({ x, y, 0.0f }), scale({ width, height }), rotation(0.0f)
        {
        }
    };

    struct SpriteComponent
    {
        SONIC_REGISTER_COMPONENT(2)

        const Sprite sprite;

        SpriteComponent(const Sprite& sprite)
            : sprite(sprite)
        {
        }
    };

    struct ColorComponent
    {
        SONIC_REGISTER_COMPONENT(2)

        Color color;

        ColorComponent(const Color& color)
            : color(color)
        {
        }

        ColorComponent(float r, float g, float b, float a)
            : color({ r, g, b, a })
        {
        }
    };

    struct ColoredSpriteComponent
    {
        SONIC_REGISTER_COMPONENT(0)

        const Color color;
        const Sprite sprite;

        ColoredSpriteComponent(const Color& color, const Sprite& sprite)
            : color(color), sprite(sprite)
        {
        }
    };

    struct Camera2DComponent
    {
        SONIC_REGISTER_COMPONENT(4)

        Camera2D camera;
        bool isSceneCamera;

        Camera2DComponent(const Camera2D& camera, bool isSceneCamera = false)
            : camera(camera), isSceneCamera(isSceneCamera)
        {
        }
    };

}
