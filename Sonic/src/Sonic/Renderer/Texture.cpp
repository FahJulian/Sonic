#include "Texture.h"
#include "stb/stb_image.h"

namespace Sonic {

    Texture::Texture(const char* filePath)
        : m_OpenGL_ID(0), m_Width(0), m_Height(0)
    {
        stbi_set_flip_vertically_on_load(1);

        unsigned char* pixelBuffer = stbi_load(filePath, &m_Width, &m_Height, NULL, 4);

        glGenTextures(1, &m_OpenGL_ID);
        glBindTexture(GL_TEXTURE_2D, m_OpenGL_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(pixelBuffer);
    }

    void Texture::Bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_OpenGL_ID);
    }

    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::Texture()
        : m_OpenGL_ID(0), m_Width(1), m_Height(1)
    {
        unsigned char pixelBuffer[]{ 0xff, 0xff, 0xff, 0xff };

        glGenTextures(1, &m_OpenGL_ID);
        glBindTexture(GL_TEXTURE_2D, m_OpenGL_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    const Texture* Texture::White()
    {
        static Texture white;
        return &white;
    }
}
