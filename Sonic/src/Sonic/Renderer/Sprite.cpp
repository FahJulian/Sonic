#include "Sprite.h"

namespace Sonic {

    Sprite::Sprite(const Texture* texture, float textureCoords[8])
        : m_Texture(texture), m_TextureCoords{ 0 }
    {
        for (int i = 0; i < 8; i++)
            m_TextureCoords[i] = textureCoords[i];
    }

    bool Sprite::IsNull() const
    {
        return m_Texture == nullptr;
    }
    
}
