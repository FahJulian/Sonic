#pragma once
#include <GL/glew.h>
#include <memory>
#include <utility>
#include <vector>
#include <iostream>

namespace Sonic {
    
    enum class ShaderDataType
    {
        Float, Float2, Float3, Float4,
        Int, Int1, Int2, Int3, Int4,
        Bool, Bool1, Bool2, Bool3, Bool4
    };

    struct BufferLayoutElement
    {
        ShaderDataType m_Type;
        int m_Offset;
        bool m_Normalized;

        BufferLayoutElement(ShaderDataType type, bool normalized = false)
            : m_Type(type), m_Offset(0), m_Normalized(normalized) {}
    };

    struct BufferLayout
    {
        BufferLayout(const std::initializer_list<BufferLayoutElement>& elements)
            : m_Elements(elements) {}
    private:
        std::vector<BufferLayoutElement> m_Elements;

        friend class VertexBuffer;
        friend class VertexArray;
    };

    class VertexBuffer
    {
    public:
        void Bind() const;
        void Unbind() const;
    protected:
        VertexBuffer(const BufferLayout& layout);
    private:
        BufferLayout m_Layout;
        unsigned int m_OpenGL_ID;
        int m_Stride;

        friend class VertexArray;
    };

    class StaticVertexBuffer : public VertexBuffer
    {
    public:
        StaticVertexBuffer(const void* data, int size, const BufferLayout& layout);
    };

    class DynamicVertexBuffer : public VertexBuffer
    {
    public:
        DynamicVertexBuffer(int maxSize, const BufferLayout& layout);

        void SetData(const void* data, int size);
    };

    static int sizeOfShaderType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float: return 1 * 4;
            case ShaderDataType::Float2: return 2 * 4;
            case ShaderDataType::Float3: return 3 * 4;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Int: return 1 * 4;
            case ShaderDataType::Int2: return 2 * 4;
            case ShaderDataType::Int3: return 3 * 4;
            case ShaderDataType::Int4: return 4 * 4;
            case ShaderDataType::Bool: return 1 * 1;
            case ShaderDataType::Bool2: return 2 * 1;
            case ShaderDataType::Bool3: return 3 * 1;
            case ShaderDataType::Bool4: return 4 * 1;
        }

        return -1;
    }

    static int primitiveTypeOfShaderType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float: return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Int: return GL_INT;
            case ShaderDataType::Int2: return GL_INT;
            case ShaderDataType::Int3: return GL_INT;
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool: return GL_BOOL;
            case ShaderDataType::Bool2: return GL_BOOL;
            case ShaderDataType::Bool3: return GL_BOOL;
            case ShaderDataType::Bool4: return GL_BOOL;
        }

        return -1;
    }

    static int componentCountOfShaderType(ShaderDataType type)
    {
        switch (type)
        {
            
            case ShaderDataType::Float: return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Int: return 1;
            case ShaderDataType::Int2: return 2;
            case ShaderDataType::Int3: return 3;
            case ShaderDataType::Int4: return 4;
            case ShaderDataType::Bool: return 1;
            case ShaderDataType::Bool2: return 2;
            case ShaderDataType::Bool3: return 3;
            case ShaderDataType::Bool4: return 4;
        }

        return -1;
    }
    
}
