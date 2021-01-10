#include "VertexBuffer.h"
#include <GL/glew.h>

namespace Sonic {
    
    VertexBuffer::VertexBuffer(const BufferLayout& layout)
        : m_Layout(layout)
    {   
        glGenBuffers(1, &m_OpenGL_ID);

        m_Stride = 0;
        for (int i = 0; i < layout.m_Elements.size(); i++)
            m_Stride += sizeOfShaderType(layout.m_Elements.at(i).m_Type);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_ID);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    StaticVertexBuffer::StaticVertexBuffer(const void* data, int size, const BufferLayout& layout)
        : VertexBuffer(layout)
    {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        Unbind();
    }

    DynamicVertexBuffer::DynamicVertexBuffer(int maxSize, const BufferLayout& layout)
        : VertexBuffer(layout)
    {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, maxSize, nullptr, GL_DYNAMIC_DRAW);
        Unbind();
    }

    void DynamicVertexBuffer::SetData(const void* data, int size)
    {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        Unbind();
    }

}
