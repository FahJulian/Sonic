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
    public:
        Sprite()
            : texture(Texture()), x0(0), x1(1), y0(0), y1(1) {}

        /**
        * Constructs a new Sprite
        * 
        * @param texture Reference to the texture the sprite is on
        * @param textureCoords float Array with the texture coordinates ot the 
        *                      sprite on the texture
        */
        Sprite(const Texture& texture, float x0, float x1, float y0, float y1)
            : texture(texture), x0(x0), x1(x1), y0(y0), y1(y1) 
        {
        }

        Sprite(const Texture& texture)
            : texture(texture), x0(0.0f), x1(1.0f), y0(0.0f), y1(1.0f)
        {
        }

        Sprite(const Sprite& other)
            : texture(other.texture), x0(other.x0), x1(other.x1), y0(other.y0), y1(other.y1) 
        {
        }

        Sprite operator=(const Sprite& other)
        {
            return Sprite(other);
        }

        bool IsNull() const { return texture.IsNull(); }

        const Texture texture;
        const float x0, x1, y0, y1;
    };

}
