#pragma once
#include "Sonic/Base.h"
#include <glm/glm.hpp>

namespace Sonic {

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

}
