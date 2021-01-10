#pragma once
#include <memory>
#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Sonic {

    /**
    * A vertex array holds an IndexBuffer and one or more VertexBuffers.
    * Handles the index buffer and gl vertex attrib arrays
    */
    class VertexArray
    {
    public:
        /**
        * Constructs a new VertexArray
        */
        VertexArray();

        /**
        * Binds the vao to opengl
        */
        void Bind() const;

        /**
        * Unbinds all vaos from opengl
        */
        void Unbind() const;

        /**
        * Binds the given IndexBuffer to this vao
        */
        void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

        /**
        * Adds the given vertex buffer and adjusts the vertex attrib pointers
        * accordingly
        */
        void AddVertexBuffer(const VertexBuffer& vertexBuffer);

        const IndexBuffer& GetIndexBuffer() const { return *m_IndexBuffer; }
    private:
        unsigned int m_OpenGL_ID;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        std::vector<VertexBuffer> m_VertexBuffers;

        void UpdateVertexAttribPointers() const;
    };

}
