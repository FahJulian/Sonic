#pragma once
#include <GL/glew.h>
#include <string>

namespace Sonic {

    class Texture
    {
    public:
        Texture(const char* filePath);
        Texture(const std::string& filePath)
            : Texture(filePath.c_str()) 
        {
        }
        Texture()
            : m_OpenGL_ID(0), m_Width(0), m_Height(0)
        {
        }

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        bool operator==(const Texture& other) const { return other.m_OpenGL_ID == this->m_OpenGL_ID; }

    private:
        unsigned int m_OpenGL_ID;
        int m_Width;
        int m_Height;
    };

}
