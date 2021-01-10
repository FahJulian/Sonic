#pragma once
#include "Texture.h"

namespace Sonic {

    class Sprite
    {
    public:
        Sprite(const Texture& texture, const float textureCoords[8]);

    public:
        const Texture& GetTexture() const { return m_Texture; }
        const float* GetTextureCoords() const { return m_TextureCoords; }
    private:
        const Texture& m_Texture;
        float m_TextureCoords[8];
    };

}
