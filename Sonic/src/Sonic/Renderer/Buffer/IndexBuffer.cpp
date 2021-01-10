#include <GL/glew.h>
#include "IndexBuffer.h"

namespace Sonic {

    IndexBuffer::IndexBuffer(const int* indices, int elementCount)
       : m_Indices(indices), m_ElementCount(elementCount)
    {
       glGenBuffers(1, &m_OpenGL_ID);
    }

    void IndexBuffer::Bind() const
    {
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OpenGL_ID);
    }

    void IndexBuffer::Unbind() const
    {
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}
