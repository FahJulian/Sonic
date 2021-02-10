#pragma once
#include <glm/glm.hpp>
#include "Sonic/Base.h"
#include "Sonic/Graphics/Color.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"

namespace Sonic {

    class Script;

    struct ScriptComponent
    {
        Script* script;

        ScriptComponent(Script* script)
            : script(script)
        {
        }
    };

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
    private:
        glm::vec3 position;
        glm::vec2 scale;
        float rotation;

    public:
        Ref<bool> rendererDirty = Ref<bool>(nullptr);

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

        void SetPosition(const glm::vec3& newPosition) { position = newPosition; if (rendererDirty) *rendererDirty = true; }
        void SetX(float newX) { position.x = newX; if (rendererDirty) *rendererDirty = true; }
        void SetY(float newY) { position.y = newY; if (rendererDirty) *rendererDirty = true; }
        void SetZ(float newZ) { position.z = newZ; if (rendererDirty) *rendererDirty = true; }

        void SetScale(const glm::vec2& newScale) { scale = newScale; if (rendererDirty) *rendererDirty = true; }
        void SetWidth(const glm::vec2& newWidth) { scale = newWidth; if (rendererDirty) *rendererDirty = true; }
        void SetHeight(const glm::vec2& newHeight) { scale = newHeight; if (rendererDirty) *rendererDirty = true; }

        void SetRotation(float newRotation) { rotation = newRotation; if (rendererDirty) *rendererDirty = true; }

        const glm::vec3& GetPosition() const { return position; }
        float GetX() const { return position.x; }
        float GetY() const { return position.y; }
        float GetZ() const { return position.z; }

        const glm::vec2& GetScale() const { return scale; }
        float GetWidth() const { return scale.x; }
        float GetHeight() const { return scale.y; }

        float GetRotation() const { return rotation; }
    };

    struct Renderer2DComponent
    {
    private:
        Sprite sprite;
        Color color;

    public:
        Ref<bool> dirty = Ref<bool>(new bool(true));

        Renderer2DComponent(const Sprite& sprite, const Color& color)
            : sprite(sprite), color(color) {}
        Renderer2DComponent(const Sprite& sprite)
            : sprite(sprite), color(Colors::White) {}
        Renderer2DComponent(const Color& color)
            : sprite(Sprite()), color(color) {}

        void SetSprite(const Sprite& newSprite) { sprite = newSprite; *dirty = true; }
        void SetColor(const Color& newColor) { color = newColor; *dirty = true; }

        const Sprite& GetSprite() const { return sprite; }
        const Color& GetColor() const { return color; }
    };

    struct Camera2DComponent
    {
        Camera2D camera;
        bool isSceneCamera;

        Camera2DComponent(const Camera2D& camera, bool isSceneCamera = false)
            : camera(camera), isSceneCamera(isSceneCamera) {}
    };

}
