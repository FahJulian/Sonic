#pragma once
#include <vector>
#include <string> 
#include "Sprite.h"
#include "Texture.h"

namespace Sonic {

    /**
    * A Spritesheet is a special Texture that contains Sprites in a grid pattern
    */
    class Spritesheet : public Texture
    {
    public:
        /**
        * Constructs a new Spritesheet.
        * 
        * @param filePath Path to the .png file of the spritesheets texture
        *
        * @param rows The number of rows of sprites on the texture
        * @param cols The number of cols of sprites on the texture
        * @param spriteWidth The width of each sprite in pixels
        * @param spriteHeight The height of each sprite in pixels
        * @param padding The padding between each sprite (Must be on the lower and right side of each sprite)
        */
        Spritesheet(const char* filePath, int rows, int cols, float spriteWidth, float spriteHeight, int padding);

        /**
        * Constructs a new Spritesheet.
        *
        * @param filePath Path to the .png file of the spritesheets texture
        *
        * @param rows The number of rows of sprites on the texture
        * @param cols The number of cols of sprites on the texture
        * @param spriteWidth The width of each sprite in pixels
        * @param spriteHeight The height of each sprite in pixels
        * @param padding The padding between each sprite (Must be on the lower and right side of each sprite)
        */
        Spritesheet(const std::string& filePath, int rows, int cols, float spriteWidth, float spriteHeight, int padding)
            : Spritesheet(filePath.c_str(), rows, cols, spriteWidth, spriteHeight, padding)
        {
        }

        /**
        * @param row The row of the sprite to return
        * @param col The col of the sprite to return
        * 
        * @return The sprite at the given coordinates on the texture
        */
        const Sprite& GetSprite(int row, int col) const;
    private:
        int m_Rows;
        int m_Cols;
        std::vector<Sprite> m_Sprites;
    };

}
