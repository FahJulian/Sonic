#pragma once

namespace Sonic {
    
    class IndexBuffer
    {
    public:
        IndexBuffer(const int* indices, int elementCount);

        void Bind() const;
        void Unbind() const;

        int GetElementCount() const { return m_ElementCount; }
    private:
        unsigned int m_OpenGL_ID;
        const int* m_Indices;
        int m_ElementCount;

        friend class VertexArray;
    };

}
