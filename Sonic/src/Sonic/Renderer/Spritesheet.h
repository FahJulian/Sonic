#pragma once
#include <vector>
#include <string> 
#include "Sprite.h"
#include "Texture.h"

namespace Sonic {

    class Spritesheet : public Texture
    {
    public:
        Spritesheet(const char* filePath, int rows, int cols, float spriteWidth, float spriteHeight, int padding);
        Spritesheet(const std::string& filePath, int rows, int cols, float spriteWidth, float spriteHeight, int padding)
            : Spritesheet(filePath.c_str(), rows, cols, spriteWidth, spriteHeight, padding)
        {
        }

        const Sprite& GetSprite(int row, int col) const;
    private:
        int m_Rows;
        int m_Cols;
        std::vector<Sprite> m_Sprites;
    };

}
