#include "Spritesheet.h"

namespace Sonic {

    Spritesheet::Spritesheet(const char* filePath, int rows, int cols, float spriteWidth, float spriteHeight, int padding)
        : Texture(filePath), m_Rows(rows), m_Cols(cols)
    {
        m_Sprites.reserve(rows * cols);

        for (int y = rows; y > 0; y--)
        {
            for (int x = 0; x < cols; x++)
            {
                float x0 = (x * (spriteWidth + padding)) / Texture::GetWidth();
                float x1 = x0 + (spriteWidth / Texture::GetWidth());
                float y1 = (y * (spriteHeight + padding)) / Texture::GetHeight();
                float y0 = y1 - (spriteHeight / Texture::GetHeight());

                float textureCoords[8] = {
                    x0, y0,
                    x1, y0,
                    x0, y1,
                    x1, y1 
                };  

                m_Sprites.emplace_back(this, textureCoords);
            }
        }
    }
 
    const Sprite& Spritesheet::GetSprite(int row, int col) const
    {
        return m_Sprites.at(col + row * m_Cols);
    }

}
