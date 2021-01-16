#pragma once
#include <GL/glew.h>
#include <string>

namespace Sonic {

    /**
    * A OpenGL Texture (Holds width and height of the texture as well as the
    * OpenGL id.
    */
    class Texture
    {
    private:
        Texture();

    public:
        /**
        * Constructs a new Texture
        * 
        * @param filePath Path to the .png file of the Texture
        */
        Texture(const char* filePath);

        /**
        * Constructs a new Texture
        *
        * @param filePath Path to the .png file of the Texture
        */
        Texture(const std::string& filePath)
            : Texture(filePath.c_str()) 
        {
        }

        static const Texture* White();

        /**
        * Binds the texture to OpenGLs Texture2D Texture slots
        * 
        * @param slot The slot to bind to
        */
        void Bind(unsigned int slot = 0) const;

        /**
        * Unbinds all Textures from OpenGLs Texture2D Texture slots
        */
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
