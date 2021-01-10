#pragma once
#include <memory>
#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Sonic {

    class VertexArray
    {
    public:
        VertexArray();

        void Bind() const;
        void Unbind() const;

        void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);
        void AddVertexBuffer(const VertexBuffer& vertexBuffer);

        const IndexBuffer& GetIndexBuffer() const { return *m_IndexBuffer; }
    private:
        unsigned int m_OpenGL_ID;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        std::vector<VertexBuffer> m_VertexBuffers;

        void UpdateVertexAttribPointers() const;
    };

}
