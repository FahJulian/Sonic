#include <stb/stb_image.h>
#include <GL/glew.h>
#include "Texture.h"

namespace Sonic {

    Texture::Texture(const char* filePath)
        : m_Width(0), m_Height(0), m_OpenGL_ID(std::make_shared<unsigned int>(0))
    {
        stbi_set_flip_vertically_on_load(1);

        unsigned char* pixelBuffer = stbi_load(filePath, &m_Width, &m_Height, NULL, 4);

        glGenTextures(1, m_OpenGL_ID.get());
        glBindTexture(GL_TEXTURE_2D, *m_OpenGL_ID);

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
        glBindTexture(GL_TEXTURE_2D, *m_OpenGL_ID);
    }

    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
