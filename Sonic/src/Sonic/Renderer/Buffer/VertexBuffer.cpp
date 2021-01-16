#include <gl/glew.h>
#include "Sonic/Log/Log.h"
#include "VertexBuffer.h"

namespace Sonic {

	VertexBuffer::VertexBuffer(int size, std::initializer_list<int> layout)
		: m_Size(size), m_Stride(0)
	{
#ifdef SONIC_DEBUG
		m_Dynamic = true;
#endif
		glGenBuffers(1, &m_OpenGL_ID);

		for (int element : layout)
		{
			m_Stride += element * sizeof(float);
			m_Layout.push_back(element);
		}

		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		Unbind();
	}

	VertexBuffer::VertexBuffer(const float* data, unsigned int size, std::initializer_list<int> layout)
		: m_Size(size), m_Stride(0)
	{
#ifdef SONIC_DEBUG
		m_Dynamic = false;
#endif
		glGenBuffers(1, &m_OpenGL_ID);

		for (int element : layout)
		{
			m_Stride += element * sizeof(float);
			m_Layout.push_back(element);
		}

		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		Unbind();
	}

	void VertexBuffer::SetData(const void* data, int size)
	{
#ifdef SONIC_DEBUG
		if (!m_Dynamic)
		{
			SONIC_LOG_DEBUG("Cant set Data of buffer: Buffer is not dynamic")
			return;
		}

		if (size > m_Size)
		{
			SONIC_LOG_DEBUG("Cant set Data of buffer: Data size is bigger than buffer size")
			return;
		}
#endif

		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		Unbind();
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_ID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
