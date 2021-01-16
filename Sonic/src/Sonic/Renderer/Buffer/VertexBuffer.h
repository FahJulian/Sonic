#pragma once
#include <vector>
#include <initializer_list>

namespace Sonic {
	
	class VertexBuffer
	{
	private:
#ifdef SONIC_DEBUG
		VertexBuffer()
			: m_OpenGL_ID(0), m_Stride(0), m_Size(0), m_Dynamic(0) {}
#else
		VertexBuffer()
			: m_OpenGL_ID(0), m_Stride(0), m_Size(0) {}
#endif

	public:
		VertexBuffer(int size, std::initializer_list<int> layout);
		VertexBuffer(const float* data, unsigned int size, std::initializer_list<int> layout);

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, int size);

		bool IsNull() const { return m_Size == 0; }

		static VertexBuffer Null() { return VertexBuffer(); }

	private:
		unsigned int m_OpenGL_ID;
		std::vector<int> m_Layout;
		int m_Stride;
		int m_Size;

#ifdef SONIC_DEBUG
		bool m_Dynamic;
#endif

		friend class VertexArray;
	};

}
