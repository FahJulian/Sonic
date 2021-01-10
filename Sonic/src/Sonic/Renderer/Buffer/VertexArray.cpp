#include "VertexArray.h"
#include <GL/glew.h>


///// Static Utility functions /////

static void createVertexAttribPointer(int n, Sonic::ShaderDataType dataType, bool normalized, int stride, intptr_t offset)
{
    glVertexAttribPointer(n, Sonic::componentCountOfShaderType(dataType), 
        Sonic::primitiveTypeOfShaderType(dataType), normalized, stride, (const void*) offset);
    glEnableVertexAttribArray(n);
}

////////////////////////////////////

namespace Sonic {

    VertexArray::VertexArray()
        : m_IndexBuffer(nullptr)
    {
        glGenVertexArrays(1, &m_OpenGL_ID);
    }

    void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer)
    {
        m_VertexBuffers.push_back(vertexBuffer);
        UpdateVertexAttribPointers();
    }

    void VertexArray::UpdateVertexAttribPointers() const
    {
        Bind();

        int attribPointerCount = 0;
        for (int i = 0; i < m_VertexBuffers.size(); i++)
        {
            const VertexBuffer& vertexBuffer = m_VertexBuffers.at(i);
            vertexBuffer.Bind();

            intptr_t offset = 0;
            for (int i = 0; i < vertexBuffer.m_Layout.m_Elements.size(); i++)
            {
                const BufferLayoutElement& element = vertexBuffer.m_Layout.m_Elements.at(i);
                createVertexAttribPointer(attribPointerCount++, element.m_Type, 
                    element.m_Normalized, vertexBuffer.m_Stride, offset);
                offset += sizeOfShaderType(element.m_Type);
                continue;
            }

            vertexBuffer.Unbind();        
        }

        Unbind();
    }

    void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
    {
        m_IndexBuffer = indexBuffer;

        Bind();
        indexBuffer->Bind();     
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->m_ElementCount * sizeof(int), indexBuffer->m_Indices, GL_STATIC_DRAW);
        indexBuffer->Unbind();
        Unbind();
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_OpenGL_ID);

        if (m_IndexBuffer)
            m_IndexBuffer->Bind();
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);

        if (m_IndexBuffer)
            m_IndexBuffer->Unbind();
    }

}
