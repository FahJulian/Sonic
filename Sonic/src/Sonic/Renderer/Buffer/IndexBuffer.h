#pragma once

namespace Sonic {
    
    /**
    * Holds an array of indices and an OpenGL id to an IndexBuffer
    */
    class IndexBuffer
    {
        /**
        * Creates a new IndexBuffer
        * 
        * @param indices Pointer to the indices array
        * @param elementCount The amount of indices in the array
        */
        IndexBuffer(const int* indices, int elementCount);

        /**
        * Binds the index buffer to opengl GL_ELEMENT_ARRAY_BUFFER
        */
        void Bind() const;

        /**
        * Unbinds all index buffers from opengl GL_ELEMENT_ARRAY_BUFFER
        */
        void Unbind() const;

        int GetElementCount() const { return m_ElementCount; }

    private:
        unsigned int m_OpenGL_ID;
        const int* m_Indices;
        int m_ElementCount;

        friend class VertexArray;
    };

}
