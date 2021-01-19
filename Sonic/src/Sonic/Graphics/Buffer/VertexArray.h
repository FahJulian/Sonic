#pragma once
#include <initializer_list>
#include "VertexBuffer.h"

namespace Sonic {

	class VertexArray
	{
	private:
		VertexArray() 
			: m_ElementCount(0), m_Vao_OpenGL_ID(0), m_Ibo_OpenGL_ID(0) {};

	public:
		VertexArray(const int* indices, unsigned int elementCount, std::initializer_list<VertexBuffer> buffers);

		void Bind() const;
		void Unbind() const;

		unsigned int GetElementCount() const { return m_ElementCount; };

		bool IsNull() const { return m_ElementCount == 0; }

		static VertexArray Null() { return VertexArray(); }

	private:
		unsigned int m_ElementCount;
		unsigned int m_Vao_OpenGL_ID;
		unsigned int m_Ibo_OpenGL_ID;
	};

}
