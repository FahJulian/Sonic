#pragma once
#include "Sonic/Graphics/Color.h"
#include "Sonic/Graphics/Sprite.h"

namespace Sonic {

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

}
