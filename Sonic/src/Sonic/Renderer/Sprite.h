#pragma once
#include "Texture.h"

namespace Sonic {

    /**
    * A Sprite is a stuct holding a reference to its texture and the texture
    * coordinates on that texture. It can be used to draw textured rectangles
    * to the screen using Renderer2D
    */
    struct Sprite
    {
    private:
        Sprite()
            : m_Texture(nullptr), m_TextureCoords{ 0 } {}

    public:
        /**
        * Constructs a new Sprite
        * 
        * @param texture Reference to the texture the sprite is on
        * @param textureCoords float Array with the texture coordinates ot the 
        *                      sprite on the texture
        */
        Sprite(const Texture* texture, float textureCoords[8]);

        static Sprite Null() { return Sprite(); }

        bool IsNull() const;

        const Texture* GetTexture() const { return m_Texture; }
        const float* GetTextureCoords() const { return m_TextureCoords; }

    private:
        const Texture* m_Texture;
        float m_TextureCoords[8];
    };

}
