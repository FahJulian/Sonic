#include <gl/glew.h>
#include "VertexArray.h"

namespace Sonic {

	VertexArray::VertexArray(const int* indices, unsigned int elementCount, std::initializer_list<VertexBuffer> buffers)
		: m_ElementCount(elementCount)
	{
		glGenVertexArrays(1, &m_Vao_OpenGL_ID);

		glBindVertexArray(m_Vao_OpenGL_ID);

		int attribPointerCount = 0;
		for (auto& buffer : buffers)
		{
			buffer.Bind();

			intptr_t offset = 0;
			for (int element : buffer.m_Layout)
			{
				glVertexAttribPointer(attribPointerCount, element, GL_FLOAT, false, buffer.m_Stride, (const void*)offset);
				glEnableVertexAttribArray(attribPointerCount);

				attribPointerCount++;
				offset += element * sizeof(float);
			}

			buffer.Unbind();
		}

		glGenBuffers(1, &m_Ibo_OpenGL_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo_OpenGL_ID);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_ElementCount * sizeof(int), indices, GL_STATIC_DRAW);

		Unbind();
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_Vao_OpenGL_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo_OpenGL_ID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
